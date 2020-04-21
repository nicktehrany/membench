#ifndef MMAP_H
#define MMAP_H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_engine(Mapping &, Arguments, Results &);
void mmap_seq_read(Mapping, Results &, int);
void mmap_rand_read(Mapping, Results &, int);
void mmap_seq_write(Mapping, Results &, int);
void mmap_rand_write(Mapping, Results &, int);
void mmap_prepare_mapping(Mapping &, Arguments);
void mmap_cleanup_mapping(Mapping);
void mmap_run_benchmark(Mapping, Arguments, Results &);
void mmap_init_file(int, int);
void mmap_prepare_map_anon(Mapping &, int);
void mmap_init_mem(Mapping);
#endif