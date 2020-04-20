#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr;
    int is_pmem;
    int is_raw_mem;
    int fsize = 0;
    int bsize = 0;
};

void seq_read(Mapping, Results &, int);
void seq_write(Mapping, Results &, int);
void prepare_mapping(Mapping &, const char *, int, Arguments);
void cleanup_mapping(Mapping);
void run_benchmark(Mapping, Arguments, Results &);
void init_file(int, int);
void prepare_raw_mem(Mapping &, int);

// Temporary
void DEBUG_LINE(char *);

#endif