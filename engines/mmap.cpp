#include "mmap.h"
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

/*
 *
 * Engine that uses mmap to map files into memory and then measures throughput by 
 * reading/writing from and to it using memcpy. Possible file options are a usual
 * file from any file system, no file (mmap will be called with MAP_ANONYMOUS),
*/
void mmap_engine(Mapping &mapping, Arguments args, Results &results)
{
    mmap_prepare_mapping(mapping, args);
    mmap_run_benchmark(mapping, args, results);
    mmap_cleanup_mapping(mapping);
    dump_results(results, args);
}

void mmap_seq_read(Mapping mapping, Results &results, int runtime)
{
    long counter = 0;
    int block_index = 0;
    auto end = std::chrono::high_resolution_clock::now();
    unsigned char *dest = new unsigned char[mapping.buflen]; // Heap memory to handle large arrays

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        // Read all blocks from mapped area, start over
        if (block_index * mapping.buflen >= mapping.fsize)
            block_index = 0;

        memcpy(dest, mapping.addr + (mapping.buflen * block_index), mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        block_index++;
        counter++;
    }

    //Calculating per second memcpy * size of memcpy and convert to MiB
    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] dest;
}

void mmap_rand_read(Mapping mapping, Results &results, int runtime)
{
    long counter = 0;
    int index_counter = 0;
    auto end = std::chrono::high_resolution_clock::now();
    unsigned char *dest = new unsigned char[mapping.buflen];
    int max_ind = mapping.fsize / mapping.buflen;
    int *block_index = new int[max_ind];

    srand(time(NULL));
    for (int i = 0; i < max_ind; i++)
        block_index[i] = rand() % max_ind;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        // Read all blocks from mapped area, start over
        if (index_counter == max_ind)
            index_counter = 0;

        memcpy(dest, mapping.addr + (mapping.buflen * block_index[index_counter]), mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] dest;
    delete[] block_index;
}

void mmap_seq_write(Mapping mapping, Results &results, int runtime)
{
    long counter = 0;
    int block_index = 0;
    unsigned char *src = new unsigned char[mapping.buflen];
    auto end = std::chrono::high_resolution_clock::now();

    srand(time(NULL));
    for (int i = 0; i < mapping.buflen; i++)
        src[i] = rand() % 256;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        if (block_index * mapping.buflen >= mapping.fsize)
            block_index = 0;
        memcpy(mapping.addr + (block_index * mapping.buflen), src, mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        block_index++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] src;
}

void mmap_rand_write(Mapping mapping, Results &results, int runtime)
{
    long counter = 0;
    int index_counter = 0;
    unsigned char *src = new unsigned char[mapping.buflen];
    std::chrono::high_resolution_clock::time_point end;
    int max_ind = mapping.fsize / mapping.buflen;
    int *block_index = new int[max_ind];

    srand(time(NULL));
    for (int i = 0; i < mapping.buflen; i++)
        src[i] = rand() % 256;

    for (int i = 0; i < max_ind; i++)
        block_index[i] = rand() % max_ind;

    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        if (index_counter == max_ind)
            index_counter = 0;
        memcpy(mapping.addr + (block_index[index_counter] * mapping.buflen), src, mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] src;
    delete[] block_index;
}

void mmap_prepare_mapping(Mapping &mapping, Arguments args)
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
        // init file to avoid mmap on empty file (BUS_ERROR)
        mmap_init_file(fd, args.fsize);

        if ((mapping.addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0)) == NULL)
        {
            perror("mmap");
            exit(1);
        }

        close(fd);

        mapping.is_pmem = 0;
        mapping.map_anon = 0;
        mapping.fsize = args.fsize;
    }
}

// Mapping is anonymous
void mmap_prepare_map_anon(Mapping &mapping, int fsize)
{
    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping.addr = (char *)mmap(0, fsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, -1, 0)) == NULL)
    {
        perror("mmap");
        exit(1);
    }

    mapping.is_pmem = 0;
    mapping.map_anon = 1;
    mapping.fsize = fsize;
    mmap_init_mem(mapping);
}

void mmap_cleanup_mapping(Mapping mapping)
{
    munmap(mapping.addr, mapping.fsize);
}

void mmap_run_benchmark(Mapping mapping, Arguments args, Results &results)
{
    mapping.buflen = args.buflen;
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
        perror("Invalid Mode");
        exit(1);
    }
}

void mmap_init_file(int fd, int fsize)
{
    // TODO BETTER INITIALIZING INEFFICIENT to create huge buf
    unsigned char *buf = new unsigned char[fsize];

    srand(time(NULL));
    for (int i = 0; i < fsize; i++)
        buf[i] = rand() % 256;

    if (write(fd, buf, fsize) < 0)
    {
        perror("File Error");
        exit(1);
    }
    delete[] buf;
}

void mmap_init_mem(Mapping mapping)
{
    // TODO BETTER INITIALIZING INEFFICIENT to create huge buf
    unsigned char *buf = new unsigned char[mapping.fsize];

    srand(time(NULL));
    for (int i = 0; i < mapping.fsize; i++)
        buf[i] = rand() % 256;

    memcpy(mapping.addr, buf, mapping.fsize);
    delete[] buf;
}