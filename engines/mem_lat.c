#include "mem_lat.h"

void mmem_lat_engine()
{
    MemMap memmap;
    init_mem(&memmap);
    walk_ptrs(memmap);
    // clean mem
    // results
}

void init_mem(MemMap *memmap)
{
    uint64_t size = 1024 * 1024 * 300;
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
    base[next_ptr] = (char *)&base[0];
    memmap->base_ptr = base;
    memmap->size = size;
}

void walk_ptrs(MemMap memmap)
{
    char **walker;
    int iters = 50000000;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t elapsed = 0;
    walker = &memmap.base_ptr[0];
    char **store = (char **)calloc(memmap.size * sizeof(char *), 1);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (int i = 0; i < iters; i++)
    {
        walker = (char **)*walker;
        store[i] = *walker; // Store walker somewhere otherwise compiler uses ADCE on char **walker when optimizing
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);

    elapsed = NANS_ELAPSED(tend, tstart);
    printf("%ld ", elapsed / iters);
}