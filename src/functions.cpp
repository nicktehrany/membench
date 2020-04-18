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
    int buf[mapping.pmem_len];
    auto end = std::chrono::high_resolution_clock::now();

    initialize_pmem(mapping);

    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        pmem_memcpy(buf, mapping.pmem_addr, mapping.pmem_len, 0);
        end = std::chrono::high_resolution_clock::now();
        counter++;
    }
    double bandwidth = counter / runtime * (double)mapping.pmem_len / 1024.0 / 1024.0;

    results.seq_read = bandwidth;
}

void seq_write(Mapping mapping, Results &results)
{
    results.seq_write = 10;
}

void prepare_mapping(Mapping &mapping, const char *dir, int pmem_len)
{

    size_t *mapped_plen = NULL;
    /* memory mapping it */
    if ((mapping.pmem_addr = (int *)pmem_map_file(dir, pmem_len, PMEM_FILE_CREATE, 0666, mapped_plen, &mapping.is_pmem)) == NULL)
    {
        perror("pmem_map");
        exit(1);
    }

    mapping.pmem_len = pmem_len;
}

void initialize_pmem(Mapping mapping)
{
    //Maybe better initializing
    int buf[mapping.pmem_len];
    for (int i = 0; i < mapping.pmem_len; i++)
        buf[i] = 12;

    pmem_memcpy(mapping.pmem_addr, buf, mapping.pmem_len, 0);
}

void cleanup_mapping(Mapping mapping)
{
    pmem_unmap(mapping.pmem_addr, mapping.pmem_len);
}