#ifndef MEM_LAT_H
#define MEM_LAT_H
#include "../src/results.h"
#include "../src/functions.h"

typedef struct MemMap
{
    char **base_ptr;
    uint64_t size;
} MemMap;

void mmem_lat_engine();
void init_mem(MemMap *);
void walk_ptrs(MemMap memmap);

#endif