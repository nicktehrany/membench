#include "mem_lat.h"
#define DEF_MEM_SIZE

void mmem_lat_engine(Arguments *args, Results *results)
{
    MemMap memmap;
    init_mem(&memmap, args);
    walk_ptrs(memmap, results, args);
    dump_results(*results, *args);
    free(memmap.base_ptr);
    memmap.base_ptr = 0;
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

    // Point last pointer back to base pointer to complete circle
    base[next_ptr] = (char *)&base[0];
    memmap->base_ptr = base;
    memmap->size = size;
    args->fsize = size;
}

void walk_ptrs(MemMap memmap, Results *results, Arguments *args)
{
    char **walker;
    uint64_t iters = (args->iterations > 0) ? args->iterations : 10000000;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t elapsed = 0;
    walker = &memmap.base_ptr[0];
    char **store = (char **)calloc(memmap.size * sizeof(char *), 1);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (uint64_t i = 0; i < iters; i++)
    {
        walker = (char **)*walker;
        store[i] = *walker; // Store walker somewhere otherwise compiler uses ADCE on it when optimizing
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);

    elapsed = NANS_ELAPSED(tend, tstart);
    results->avg_lat = elapsed / iters;
    args->iterations = iters;
    free(store);
    store = NULL;
}