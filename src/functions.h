#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr = 0;
    int is_pmem = 0;
    int map_anon = 0; // MAP_ANONYMOUS (not backed by file)
    uint64_t fsize = 0;
    uint64_t buflen = 0;
    const char *fpath = "";
};

void get_bandwidth(uint64_t, int, uint64_t, Results &);

#endif
