#include "mmap.h"
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

/*
 *
 * Engine that uses mmap to map files into memory and then measures throughput by 
 * reading/writing from and to it using memcpy. Possible file options are a usual
 * file from any file system, no file (mmap will be called with MAP_ANONYMOUS),
*/
void mmap_engine(Mapping *mapping, Arguments *args, Results *results)
{
    mmap_check_args(args);
    mmap_prepare_mapping(mapping, *args);
    mmap_run_benchmark(mapping, *args, results);
    mmap_cleanup_mapping(mapping);
    dump_results(*results, *args);
}

void mmap_seq_read(Mapping *mapping, Results *results, uint64_t runtime)
{
    uint64_t counter = 0;
    uint64_t block_index = 0;
    uint64_t elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(uint64_t), 1);

    clock_t start = clock();
    clock_t end = clock();

    while (elapsed < runtime)
    {
        // Read all blocks from mapped area, start over
        if (block_index * mapping->buflen >= mapping->fsize)
            block_index = 0;

        memcpy(dest, mapping->addr + (mapping->buflen * block_index), mapping->buflen);
        end = clock();
        elapsed = (end - start) / CLOCKS_PER_SEC;
        block_index++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping->buflen, results);
    free(dest);
    dest = NULL;
}

void mmap_rand_read(Mapping *mapping, Results *results, uint64_t runtime)
{
    uint64_t counter = 0;
    uint64_t index_counter = 0;
    uint64_t elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(uint64_t), 1);
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    int *block_index = (int *)malloc(max_ind * sizeof(int));

    srand(time(NULL));
    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = rand() % max_ind;

    clock_t start = clock();
    clock_t end = clock();

    while (elapsed < runtime)
    {
        // Read all blocks from mapped area, start over
        if (index_counter == max_ind)
            index_counter = 0;

        memcpy(dest, mapping->addr + (mapping->buflen * block_index[index_counter]), mapping->buflen);
        end = clock();
        elapsed = (end - start) / CLOCKS_PER_SEC;
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping->buflen, results);
    free(dest);
    dest = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_seq_write(Mapping *mapping, Results *results, uint64_t runtime)
{
    uint64_t counter = 0;
    uint64_t block_index = 0;
    uint64_t elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(uint64_t), 1);

    srand(time(NULL));
    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    clock_t start = clock();
    clock_t end = clock();

    while (elapsed < runtime)
    {
        if (block_index * mapping->buflen >= mapping->fsize)
            block_index = 0;
        memcpy(mapping->addr + (block_index * mapping->buflen), src, mapping->buflen);
        end = clock();
        elapsed = (end - start) / CLOCKS_PER_SEC;
        block_index++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping->buflen, results);
    free(src);
    src = NULL;
}

void mmap_rand_write(Mapping *mapping, Results *results, uint64_t runtime)
{
    uint64_t counter = 0;
    uint64_t index_counter = 0;
    uint64_t elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(uint64_t), 1);
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    int *block_index = (int *)malloc(max_ind * sizeof(int));

    srand(time(NULL));
    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = rand() % max_ind;

    clock_t start = clock();
    clock_t end = clock();

    while (elapsed < runtime)
    {
        if (index_counter == max_ind)
            index_counter = 0;
        memcpy(mapping->addr + (block_index[index_counter] * mapping->buflen), src, mapping->buflen);
        end = clock();
        elapsed = (end - start) / CLOCKS_PER_SEC;
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping->buflen, results);
    free(src);
    src = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_prepare_mapping(Mapping *mapping, Arguments args)
{
    if (args.map_anon)
        mmap_prepare_map_anon(mapping, args.fsize);
    else
    {
        int fd;
        if ((fd = open(args.path, O_CREAT | O_RDWR, 0666)) < 0)
        {
            perror("File Open");
            exit(1);
        }

        // Init file manually in case fle is on DAX-fs, where truncate init isn't enough
        mmap_init_file(fd, args.fsize, args.buflen);

        if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            remove(args.path);
            exit(1);
        }

        close(fd);

        mapping->is_pmem = 0;
        mapping->map_anon = 0;
        mapping->fsize = args.fsize;
        mapping->fpath = args.path;
    }
}

// // Mapping is anonymous
void mmap_prepare_map_anon(Mapping *mapping, int fsize)
{
    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, fsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, -1, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    mapping->is_pmem = 0;
    mapping->map_anon = 1;
    mapping->fsize = fsize;
    mmap_init_mem(mapping);
}

void mmap_cleanup_mapping(Mapping *mapping)
{
    munmap(mapping->addr, mapping->fsize);

    if (!mapping->map_anon && remove(mapping->fpath) != 0)
    {
        errno = EINVAL;
        perror("Error deleting file");
    }

    mapping->addr = 0;
    mapping->buflen = 0;
    mapping->fpath = "";
    mapping->fsize = 0;
    mapping->is_pmem = 0;
    mapping->map_anon = 0;
}

void mmap_run_benchmark(Mapping *mapping, Arguments args, Results *results)
{
    mapping->buflen = args.buflen;
    switch (args.mode)
    {
    case 0:
        mmap_seq_read(mapping, results, args.runtime);
        break;
    case 1:
        mmap_seq_write(mapping, results, args.runtime);
        break;
    case 2:
        mmap_rand_read(mapping, results, args.runtime);
        break;
    case 3:
        mmap_rand_write(mapping, results, args.runtime);
        break;
    default:
        errno = EINVAL;
        perror("Invalid Mode");
        exit(1);
    }
}

void mmap_init_mem(Mapping *mapping)
{
    srand(time(NULL));
    memset(mapping->addr, rand(), mapping->fsize);
}

void mmap_check_args(Arguments *args)
{
    if (strcmp(args->path, "") == 0)
    {
        args->path = "file";
    }
}

void mmap_init_file(int fd, int fsize, int buflen)
{
    // Since file size has to be multiple of buflen
    unsigned char *buf = (unsigned char *)malloc(buflen * sizeof(char));
    int iter = fsize / buflen;

    srand(time(NULL));
    for (int i = 0; i < iter; i++)
    {
        for (int j = 0; j < buflen; j++)
            buf[j] = rand() % 256;
        if (write(fd, buf, buflen) < 0)
        {
            perror("File Error");
            exit(1);
        }
    }
    free(buf);
    buf = NULL;
}
