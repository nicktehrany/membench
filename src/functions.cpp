#include "functions.h"
#include <libpmem.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libpmem.h>
#include <iostream>

#include <chrono>

void seq_read(Mapping mapping, Results &results, int runtime)
{
    int counter = 0;
    auto end = std::chrono::high_resolution_clock::now();
    unsigned char buf[mapping.pmem_len];

    initialize_pmem(mapping);

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        buf[mapping.pmem_len] = {0};
        memcpy(buf, mapping.pmem_addr, mapping.pmem_len);
        end = std::chrono::high_resolution_clock::now();
        counter++;
    }

    //Calculating per second memcpy * size of memcpy convert to MiB
    double bandwidth = counter / runtime * (double)mapping.pmem_len / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void seq_write(Mapping mapping, Results &results, int runtime)
{
    int counter = 0;
    unsigned char buf[mapping.pmem_len];
    auto end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < mapping.pmem_len; i++)
        buf[i] = 'b';

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        pmem_memmove_persist(mapping.pmem_addr, buf, mapping.pmem_len);
        end = std::chrono::high_resolution_clock::now();
        // pmem_memset(mapping.pmem_addr, 0, mapping.pmem_len, 0);
        counter++;
    }

    //Calculating per second memcpy * size of memcpy convert to MiB
    double bandwidth = counter / runtime * (double)mapping.pmem_len / 1024.0 / 1024.0;

    results.bandwidth = bandwidth;
}

void prepare_mapping(Mapping &mapping, const char *dir, int pmem_len)
{

    size_t *mapped_plen = NULL;
    /* memory mapping it */
    if ((mapping.pmem_addr = (int *)pmem_map_file(dir, pmem_len, PMEM_FILE_CREATE, 0666,
                                                  mapped_plen, &mapping.is_pmem)) == NULL)
    {
        perror("pmem_map");
        exit(1);
    }

    mapping.pmem_len = pmem_len;
}

void initialize_pmem(Mapping mapping)
{
    //Maybe better initializing
    unsigned char buf[mapping.pmem_len];
    for (int i = 0; i < mapping.pmem_len; i++)
        buf[i] = 'b';

    pmem_memcpy_persist(mapping.pmem_addr, buf, mapping.pmem_len);
}

void cleanup_mapping(Mapping mapping)
{
    pmem_unmap(mapping.pmem_addr, mapping.pmem_len);
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