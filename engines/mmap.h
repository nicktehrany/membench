#ifndef MMAP_H
#define MMAP_H
#include "../src/results.h"
#include "../src/functions.h"

struct Mapping
{
    char *addr;
    int is_pmem;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    int fsize = 0;
    int buflen = 0;
};

void mmap_engine(Mapping &, Arguments, Results &);
void seq_read(Mapping, Results &, int);
void rand_read(Mapping, Results &, int);
void seq_write(Mapping, Results &, int);
void prepare_mapping(Mapping &, Arguments);
void cleanup_mapping(Mapping);
void run_benchmark(Mapping, Arguments, Results &);
void init_file(int, int);
void prepare_map_anon(Mapping &, int);
void init_mem(Mapping);
#endif