#include "functions.h"
#include <libpmem.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include <chrono>

void seq_read(Mapping mapping, Results &results, int runtime)
{
    int counter = 0;
    int block_index = 0;
    auto end = std::chrono::high_resolution_clock::now();
    unsigned char buf[mapping.bsize] = {0};

    initialize_mem(mapping);

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        // Read all blocks from mapped area, start over
        if (block_index * mapping.bsize >= mapping.fsize)
            block_index = 0;
        memcpy(buf, mapping.addr + (mapping.bsize * block_index), mapping.bsize);
        end = std::chrono::high_resolution_clock::now();
        block_index++;
        counter++;
    }

    //Calculating per second memcpy * size of memcpy and convert to MiB
    double bandwidth = counter / runtime * (double)mapping.bsize / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void seq_write(Mapping mapping, Results &results, int runtime)
{
    int counter = 0;
    int block_index = 0;
    unsigned char buf[mapping.bsize];
    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < mapping.bsize; i++)
        buf[i] = 'b';

    auto start = std::chrono::high_resolution_clock::now();

    // If mapping is raw pmem use libpmem functions
    if (mapping.is_pmem)
    {
        while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
        {
            if (block_index * mapping.bsize >= mapping.fsize)
                block_index = 0;
            pmem_memcpy_persist(mapping.addr + (block_index * mapping.bsize), buf, mapping.bsize);
            end = std::chrono::high_resolution_clock::now();
            block_index++;
            counter++;
        }
    }
    // Else use regular functions
    else
    {
        while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
        {
            if (block_index * mapping.bsize >= mapping.fsize)
                block_index = 0;
            memcpy(mapping.addr + (block_index * mapping.bsize), buf, mapping.bsize);
            end = std::chrono::high_resolution_clock::now();
            block_index++;
            counter++;
        }
    }

    //Calculating per second memcpy * size of memcpy and convert to MiB
    double bandwidth = counter / runtime * (double)mapping.fsize / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void prepare_mapping(Mapping &mapping, const char *dir, int fsize, int raw_pmem)
{

    // Supporting raw persistent memory access
    if (raw_pmem)
    {
        size_t *mapped_plen = NULL;
        /* memory mapping it */
        if ((mapping.addr = (char *)pmem_map_file(dir, fsize, PMEM_FILE_CREATE, 0666,
                                                  mapped_plen, &mapping.is_pmem)) == NULL)
        {
            perror("pmem_map");
            exit(1);
        }
    }
    // All other accesses (DAX-mmap or mmap)
    else
    {
        int fd;
        //Add file path from args
        if ((fd = open(dir, O_CREAT | O_RDWR, 0666)) < 0)
        {
            perror("File Open");
            exit(1);
        }
        // init file to avoid mmap on empty file (BUS_ERROR)
        initialize_file(fd, fsize);

        if ((mapping.addr = (char *)mmap(0, fsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0)) == NULL)
        {
            perror("mmap");
            exit(1);
        }

        close(fd);

        mapping.is_pmem = 0;
    }
    mapping.fsize = fsize;
}

void initialize_mem(Mapping mapping)
{
    //TODO better initializing INEFFICIENT to create huge buf
    unsigned char *buf = new unsigned char[mapping.fsize]; //Handle large sizes

    for (int i = 0; i < mapping.fsize; i++)
        buf[i] = 'b';

    if (mapping.is_pmem)
        pmem_memcpy_persist(mapping.addr, buf, mapping.fsize);
    else
        memcpy(mapping.addr, buf, mapping.fsize);
    delete[] buf;
}

void cleanup_mapping(Mapping mapping)
{
    if (mapping.is_pmem)
        pmem_unmap(mapping.addr, mapping.fsize);
    else
        munmap(mapping.addr, mapping.fsize);
}

void run_benchmark(Mapping mapping, Arguments args, Results &results)
{
    switch (args.mode)
    {
    case 0:
        seq_read(mapping, results, args.runtime);
        break;
    case 1:
        seq_write(mapping, results, args.runtime);
    case 2:
        //random read
        break;
    case 3:
        //random write
        break;
    default:
        perror("Invalid Mode");
        exit(1);
    }
}

void initialize_file(int fd, int fsize)
{
    unsigned char *buf = new unsigned char[fsize]; // Handle large sizes

    // TODO BETTER INITIALIZING
    for (int i = 0; i < fsize; i++)
        buf[i] = 'b';

    if (write(fd, buf, fsize) < 0)
    {
        perror("File Error");
        exit(1);
    }
    delete[] buf;
}

// TEMPORARY
void DEBUG_LINE(char *msg)
{
    std::cout << msg << std::endl;
}