#include "mmap.h"
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#define NANS_TO_SECS 1.0e-9
#define SECS_TO_NANS 1.0e9

/*
 *
 * Engine that uses mmap to map files into memory and then measures throughput by 
 * reading/writing from and to it using memcpy. Possible file options are a usual
 * file from any mounted file system, MAP_ANONYMOUS for no file backing.
 * Engine will create and delete a file of the specified sizes (which can take longer 
 * for large files). Will memcpy entire file at least once, unless iterations are 
 * specified, can therefore take longer than runtime if file is large and copy size
 * is small
*/
void mmap_engine(Mapping *mapping, Arguments *args, Results *results)
{
    mmap_check_args(args);
    mmap_prepare_mapping(mapping, *args);
    mmap_run_benchmark(mapping, args, results);
    mmap_cleanup_mapping(mapping);
    dump_results(*results, *args);
}

void mmap_seq_read(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, index_counter = 0, elapsed = 0;
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    long double secs_elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (mapping->buflen * i));

    struct timespec tstart = {0, 0}, tend = {0, 0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    while (secs_elapsed < args->runtime)
    {
        // Read all blocks from mapped area, start over
        // Don't want to gettime after every copy, only once entire file is copied
        if (index_counter == max_ind || args->iterations == counter)
        {
            index_counter = 0;
            clock_gettime(CLOCK_MONOTONIC, &tend);
            elapsed = ((tend.tv_sec - tstart.tv_sec) * SECS_TO_NANS) + (tend.tv_nsec - tstart.tv_nsec);
            add_latency(elapsed / (args->fsize / args->buflen), results);
            secs_elapsed += ((double)tend.tv_sec + NANS_TO_SECS * tend.tv_nsec) -
                            ((double)tstart.tv_sec + NANS_TO_SECS * tstart.tv_nsec);
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            if (args->iterations != 0 && args->iterations == counter)
                break;
        }

        memcpy(dest, block_index[index_counter], mapping->buflen * sizeof(char));
        index_counter++;
        counter++;
    }
    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;

    // In case runtime deviated from desired runtime (unlikely but possible on very large memcpy)
    args->runtime = secs_elapsed;
    free(dest);
    dest = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_rand_read(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, index_counter = 0, elapsed = 0;
    long double secs_elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    srand(time(NULL));
    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (mapping->buflen * (rand() % max_ind)));

    struct timespec tstart = {0, 0}, tend = {0, 0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    while (secs_elapsed < args->runtime)
    {
        if (index_counter == max_ind || args->iterations == counter)
        {
            index_counter = 0;
            clock_gettime(CLOCK_MONOTONIC, &tend);
            elapsed = ((tend.tv_sec - tstart.tv_sec) * SECS_TO_NANS) + (tend.tv_nsec - tstart.tv_nsec);
            add_latency(elapsed / (args->fsize / args->buflen), results);
            secs_elapsed += ((double)tend.tv_sec + NANS_TO_SECS * tend.tv_nsec) -
                            ((double)tstart.tv_sec + NANS_TO_SECS * tstart.tv_nsec);
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            if (args->iterations != 0 && args->iterations == counter)
                break;
        }

        memcpy(dest, block_index[index_counter], mapping->buflen * sizeof(char));
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->runtime = secs_elapsed;

    free(dest);
    dest = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_seq_write(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, elapsed = 0;
    long double secs_elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    uint64_t index_counter = 0;
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (mapping->buflen * i));

    srand(time(NULL));
    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    struct timespec tstart = {0, 0}, tend = {0, 0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    while (secs_elapsed < args->runtime)
    {
        if (index_counter == max_ind || args->iterations == counter)
        {
            index_counter = 0;
            clock_gettime(CLOCK_MONOTONIC, &tend);
            elapsed = ((tend.tv_sec - tstart.tv_sec) * SECS_TO_NANS) + (tend.tv_nsec - tstart.tv_nsec);
            add_latency(elapsed / (args->fsize / args->buflen), results);
            secs_elapsed += ((double)tend.tv_sec + NANS_TO_SECS * tend.tv_nsec) -
                            ((double)tstart.tv_sec + NANS_TO_SECS * tstart.tv_nsec);
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            if (args->iterations != 0 && args->iterations == counter)
                break;
        }

        memcpy(block_index[index_counter], src, mapping->buflen * sizeof(char));
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / (double)counter;
    args->runtime = secs_elapsed;

    free(src);
    src = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_rand_write(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, index_counter = 0, elapsed = 0;
    long double secs_elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    uint64_t max_ind = mapping->fsize / mapping->buflen;
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    srand(time(NULL));
    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (mapping->buflen * (rand() % max_ind)));

    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    struct timespec tstart = {0, 0}, tend = {0, 0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    while (secs_elapsed < args->runtime)
    {
        if (index_counter == max_ind || args->iterations == counter)
        {
            index_counter = 0;
            clock_gettime(CLOCK_MONOTONIC, &tend);
            elapsed = ((tend.tv_sec - tstart.tv_sec) * SECS_TO_NANS) + (tend.tv_nsec - tstart.tv_nsec);
            add_latency(elapsed / (args->fsize / args->buflen), results);
            secs_elapsed += ((double)tend.tv_sec + NANS_TO_SECS * tend.tv_nsec) -
                            ((double)tstart.tv_sec + NANS_TO_SECS * tstart.tv_nsec);
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            if (args->iterations != 0 && args->iterations == counter)
                break;
        }

        memcpy(block_index[index_counter], src, mapping->buflen * sizeof(char));
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->runtime = secs_elapsed;

    free(src);
    src = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_prepare_mapping(Mapping *mapping, Arguments args)
{
    if (args.map_anon)
        mmap_prepare_map_anon(mapping, args);
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

        int flags = set_flags(args);
        if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
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

// Mapping is anonymous
void mmap_prepare_map_anon(Mapping *mapping, Arguments args)
{
    int flags = set_flags(args);

    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, -1, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    mapping->is_pmem = 0;
    mapping->map_anon = 1;
    mapping->fsize = args.fsize;

    if (args.init_mem)
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

void mmap_run_benchmark(Mapping *mapping, Arguments *args, Results *results)
{
    mapping->buflen = args->buflen;
    switch (args->mode)
    {
    case 0:
        mmap_seq_read(mapping, results, args);
        break;
    case 1:
        mmap_seq_write(mapping, results, args);
        break;
    case 2:
        mmap_rand_read(mapping, results, args);
        break;
    case 3:
        mmap_rand_write(mapping, results, args);
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

// Check if all args are valid for engine to start
void mmap_check_args(Arguments *args)
{
    if (strcmp(args->path, "") == 0)
    {
        args->path = "file";
    }
    if (args->buflen <= 0 || args->fsize <= 0)
    {
        errno = EINVAL;
        perror("Invalid or missing file or copy size");
        exit(1);
    }
    if (args->runtime < 1)
    {
        errno = EINVAL;
        perror("Runtime should be greater than 1sec");
        exit(1);
    }
    if (args->buflen > args->fsize)
    {
        errno = EINVAL;
        perror("Copy size can't be larger than file size");
        exit(1);
    }
    if (args->fsize % args->buflen != 0)
    {
        errno = EINVAL;
        perror("Not aligned file size and copy size");
        exit(1);
    }
}

/*
*  For performance reasons, don't want to write to file in single bytes at a time (which
*  will take very long for large files), write in 4KiB amounts at a time or buflen 
*  (copy size) amounts if it's larger
*/
void mmap_init_file(int fd, int fsize, int buflen)
{
    // TODO Some smarter way of init for very large files
    int factor = 32768;
    if (buflen > factor)
        factor = buflen;
    if (fsize < factor)
        factor = fsize;

    // Since file size has to be multiple of buflen
    unsigned char *buf = (unsigned char *)malloc(factor * sizeof(char));
    int iter = fsize / factor;

    srand(time(NULL));
    for (int i = 0; i < iter; i++)
    {
        for (int j = 0; j < factor; j++)
            buf[j] = rand() % 256;
        if (write(fd, buf, factor) < 0)
        {
            perror("File Error");
            exit(1);
        }
    }
    // Make sure data is written back to fs
    fsync(fd);
    free(buf);
    buf = NULL;
}
