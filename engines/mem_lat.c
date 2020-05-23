#include "mem_lat.h"
#define DEF_WALKS 10000000

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
    results.avg_lat = ((runtime / (double)iters) * SECS_TO_NANS) / (double)DEF_WALKS;
    dump_results(results, *args);
}

void init_mem(MemMap *memmap, Arguments *args)
{
    uint64_t size = (args->fsize > 0) ? args->fsize : 1024 * 1024 * 300; // Default 300MiB memory allocation
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
    args->fsize = size;
}

double walk_ptrs(MemMap memmap, Results *results)
{
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t elapsed = 0;
    char **walker = &memmap.base_ptr[0];
    char **store = (char **)calloc(memmap.size * sizeof(char *), 1);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (uint64_t i = 0; i < DEF_WALKS; i++)
    {
        walker = (char **)*walker;
        store[i % memmap.size] = *walker; // Store walking pointer somewhere otherwise compiler uses ADCE on it when optimizing
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);

    elapsed = NANS_ELAPSED(tend, tstart);
    add_latency(elapsed / DEF_WALKS, results);
    free(store);
    store = NULL;

    return elapsed * NANS_TO_SECS;
}