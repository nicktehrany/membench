#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr;
    int is_pmem;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    int fsize = 0;
    int buflen = 0;
};

void get_bandwidth(int, int, int, Results &);

#endif
