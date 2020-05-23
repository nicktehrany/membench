#ifndef MEM_LAT_H
#define MEM_LAT_H
#include "../src/results.h"
#include "../src/functions.h"

typedef struct MemMap
{
    char **base_ptr;
    uint64_t size;
} MemMap;

void mem_lat_engine(Arguments *args);
void init_mem(MemMap *memmap, Arguments *args);
double walk_ptrs(MemMap memmap, Results *results);

#endif