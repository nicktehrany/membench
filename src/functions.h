#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

typedef struct Mapping
{
    char *addr;
    int is_pmem;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    uint64_t fsize;
    uint64_t buflen;
    const char *fpath;
} Mapping;

void get_bandwidth(uint64_t, int, uint64_t, Results *);
#endif
