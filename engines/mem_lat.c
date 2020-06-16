#include "mem_lat.h"
#define DEF_STEPS 10000000

/*
 *
 * This Engine is meant to measure the latency of memory. It achieves this 
 * by creating a ring of pointers in a malloced buffer in memory, which are 
 * then walked along. Allocation works by starting with the base pointer, 
 * setting it to point to some other random address in the buffer, then the 
 * pointer at that address will be set to point to some other random address,
 * and so on. The last pointer to be set will point back to the base pointer, 
 * therefore not every pointer in the buffer may point to an address, but this
 * will create a cycle that can be walked multiple times. The walk will start 
 * at the base pointer and it'll keep going to the next pointer 10,000,000 times. 
 */
void mem_lat_engine(Arguments *args)
{
    Results results = {0, 0, 0, 0, 0};
    MemMap memmap;
    double runtime = 0.0;
    uint64_t iters = (args->iterations > 0) ? args->iterations : 1;
    for (uint64_t i = 0; i < iters; i++)
    {
        init_mem(&memmap, args);
        runtime += walk_ptrs(memmap, &results);
        free(memmap.base_ptr);
        memmap.base_ptr = NULL;
    }

    args->iterations = iters;
    if (args->iterations == 1)
        results.avg_lat = results.max_lat;
    else
        results.avg_lat = ((runtime / (double)iters) * SECS_TO_NANS) / (double)DEF_STEPS;
    dump_results(results, *args);
}

void init_mem(MemMap *memmap, Arguments *args)
{
    uint64_t size = (args->size > 0) ? args->size : 1024 * 1024 * 300; // Default 300MiB memory allocation
    char **base = (char **)calloc(size * sizeof(char *), 1);
    int next_ptr = 0, temp;

    base[0] = (char *)&base[rand() % size];
    next_ptr = 0;

    srand(time(NULL));
    for (uint64_t i = 0; i < size - 1; i++)
    {
        temp = rand() % size;
        base[next_ptr] = (char *)&base[temp];
        next_ptr = temp;
    }

    // Point last pointer back to base pointer to complete cycle
    base[next_ptr] = (char *)&base[0];
    memmap->base_ptr = base;
    memmap->size = size;
    args->size = size;
}

double walk_ptrs(MemMap memmap, Results *results)
{
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t elapsed = 0;
    char **walker = &memmap.base_ptr[0];
    char **store = (char **)calloc(memmap.size * sizeof(char *), 1);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (uint64_t i = 0; i < DEF_STEPS; i++)
    {
        walker = (char **)*walker;
        store[i % memmap.size] = *walker; // Store walking pointer somewhere otherwise compiler uses ADCE when optimizing
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);

    elapsed = NANS_ELAPSED(tend, tstart);
    add_latency(elapsed / DEF_STEPS, results);
    free(store);
    store = NULL;

    return elapsed * NANS_TO_SECS;
}