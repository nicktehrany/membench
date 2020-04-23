#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr;
    int is_pmem;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    long fsize = 0;
    long buflen = 0;
    const char *fpath = "";
};

void get_bandwidth(int, int, int, Results &);

#endif
