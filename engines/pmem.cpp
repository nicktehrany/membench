#include "pmem.h"
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <libpmem.h>

/*
 *
 * Engine that uses libpmem functions to access raw persistent memory, 
 * which needs to be mounted with a DAX enabled filesystem, and measures
 * throughput of sequential and random reads and writes
*/
void Eng_pmem::pmem_engine(Mapping &mapping, Arguments args, Results &results)
{
    check_args(args);
    prepare_mapping(mapping, args);
    run_benchmark(mapping, args, results);
    cleanup_mapping(mapping);
    dump_results(results, args);
}

// TODO Check if all args are valid for engine
void Eng_pmem::check_args(Arguments args)
{
    if (args.map_anon)
    {
        perror("Pmem engine doesn't support MAP_ANONYMOUS");
        exit(1);
    }
}

void Eng_pmem::run_benchmark(Mapping mapping, Arguments args, Results &results)
{
    mapping.buflen = args.buflen;
    switch (args.mode)
    {
    case 0:
        seq_read(mapping, results, args.runtime);
        break;
    case 1:
        seq_write(mapping, results, args.runtime);
        break;
    case 2:
        rand_read(mapping, results, args.runtime);
        break;
    case 3:
        rand_write(mapping, results, args.runtime);
        break;
    default:
        perror("Invalid Mode");
        exit(1);
    }
}

void Eng_pmem::seq_read(Mapping mapping, Results &results, int runtime)
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

void Eng_pmem::rand_read(Mapping mapping, Results &results, int runtime)
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

void Eng_pmem::seq_write(Mapping mapping, Results &results, int runtime)
{
    long counter = 0;
    int block_index = 0;
    unsigned char *src = new unsigned char[mapping.buflen];
    auto end = std::chrono::high_resolution_clock::now();

    srand(time(NULL));
    for (int i = 0; i < mapping.buflen; i++)
        src[i] = rand() % 256;

    auto start = std::chrono::high_resolution_clock::now();

    // If mapping is raw pmem use libpmem functions
    while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < runtime)
    {
        if (block_index * mapping.buflen >= mapping.fsize)
            block_index = 0;
        pmem_memcpy(mapping.addr + (block_index * mapping.buflen), src, mapping.buflen, 0);
        pmem_persist(mapping.addr, mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        block_index++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] src;
}

void Eng_pmem::rand_write(Mapping mapping, Results &results, int runtime)
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
        pmem_memcpy(mapping.addr + (block_index[index_counter] * mapping.buflen), src, mapping.buflen, 0);
        pmem_persist(mapping.addr, mapping.buflen);
        end = std::chrono::high_resolution_clock::now();
        index_counter++;
        counter++;
    }

    get_bandwidth(counter, runtime, mapping.buflen, results);
    delete[] src;
    delete[] block_index;
}

void Eng_pmem::prepare_mapping(Mapping &mapping, Arguments args)
{
    size_t *mapped_plen = NULL;
    /* memory mapping it */
    if ((mapping.addr = (char *)pmem_map_file(args.path, args.fsize, PMEM_FILE_CREATE, 0666,
                                              mapped_plen, &mapping.is_pmem)) == NULL)
    {
        perror("pmem_map");
        exit(1);
    }
    mapping.fsize = args.fsize;
}

// TODO reset mapping vars
void Eng_pmem::cleanup_mapping(Mapping mapping)
{
    pmem_unmap(mapping.addr, mapping.fsize);
}