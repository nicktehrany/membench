#ifndef MMAP_H
#define MMAP_H
#include "../src/results.h"
#include "../src/functions.h"

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
void prepare_mapping(Mapping &, Arguments);
void cleanup_mapping(Mapping);
void run_benchmark(Mapping, Arguments, Results &);
void init_file(int, int);
void prepare_raw_mem(Mapping &, int);
void init_mem(Mapping);

// Temporary
void DEBUG_LINE(char *);

#endif