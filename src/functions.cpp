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
    auto end = std::chrono::high_resolution_clock::now();
    unsigned char buf[mapping.len] = {0};

    initialize_mem(mapping);

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        memcpy(buf, mapping.addr, mapping.len);
        end = std::chrono::high_resolution_clock::now();
        counter++;
    }

    //Calculating per second memcpy * size of memcpy and convert to MiB
    double bandwidth = counter / runtime * (double)mapping.len / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void seq_write(Mapping mapping, Results &results, int runtime)
{
    int counter = 0;
    unsigned char buf[mapping.len];
    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < mapping.len; i++)
        buf[i] = 'b';

    auto start = std::chrono::high_resolution_clock::now();

    // If mapping is raw pmem use libpmem functions
    if (mapping.is_pmem)
    {
        while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
        {
            pmem_memcpy_persist(mapping.addr, buf, mapping.len);
            end = std::chrono::high_resolution_clock::now();
            // pmem_memset(mapping.addr, 0, mapping.len, 0); WRONG BUT HELPS
            counter++;
        }
    }
    // Else use regular functions
    else
    {
        while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
        {
            memcpy(mapping.addr, buf, mapping.len);
            end = std::chrono::high_resolution_clock::now();
            counter++;
        }
    }

    //Calculating per second memcpy * size of memcpy and convert to MiB
    double bandwidth = counter / runtime * (double)mapping.len / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void prepare_mapping(Mapping &mapping, const char *dir, int len, int raw_pmem)
{

    // Supporting raw persistent memory access
    if (raw_pmem)
    {
        size_t *mapped_plen = NULL;
        /* memory mapping it */
        if ((mapping.addr = (int *)pmem_map_file(dir, len, PMEM_FILE_CREATE, 0666,
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
        initialize_file(fd, len);

        if ((mapping.addr = (int *)mmap(0, len, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0)) == NULL)
        {
            perror("mmap");
            exit(1);
        }

        close(fd);

        mapping.is_pmem = 0;
    }
    mapping.len = len;
}

void initialize_mem(Mapping mapping)
{
    //TODO better initializing
    unsigned char buf[mapping.len];
    for (int i = 0; i < mapping.len; i++)
        buf[i] = 'b';

    if (mapping.is_pmem)
        pmem_memcpy_persist(mapping.addr, buf, mapping.len);
    else
        memcpy(mapping.addr, buf, mapping.len);
}

void cleanup_mapping(Mapping mapping)
{
    pmem_unmap(mapping.addr, mapping.len);
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

void initialize_file(int fd, int len)
{
    unsigned char buf[len];

    // TODO BETTER INITIALIZING
    for (int i = 0; i < len; i++)
        buf[i] = 'b';

    if (write(fd, buf, len) < 0)
    {
        perror("File Error");
    }
}

// TEMPORARY
void DEBUG_LINE(char *msg)
{
    std::cout << msg << std::endl;
}