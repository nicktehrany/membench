#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr = 0;
    int is_pmem = 0;
    int map_anon = 0; // MAP_ANONYMOUS (not backed by file)
    long fsize = 0;
    long buflen = 0;
    const char *fpath = "";
};

void get_bandwidth(int, int, int, Results &);

#endif
