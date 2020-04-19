#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    char *addr;
    int is_pmem;
    int fsize = 0;
    int bsize = 0;
};

void seq_read(Mapping, Results &, int);
void seq_write(Mapping, Results &, int);
void prepare_mapping(Mapping &, const char *, int, int);
void initialize_mem(Mapping);
void cleanup_mapping(Mapping);
void run_benchmark(Mapping, Arguments, Results &);
void initialize_file(int, int);

// Temporary

void DEBUG_LINE(char *);

#endif