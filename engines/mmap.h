#ifndef H
#define H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_engine(Mapping *, Arguments *, Results *);
void mmap_check_args(Arguments *);
void mmap_seq_read(Mapping *mapping, Results *results, uint64_t runtime);
void mmap_rand_read(Mapping *, Results *, uint64_t);
void mmap_seq_write(Mapping *, Results *, uint64_t);
void mmap_rand_write(Mapping *, Results *, uint64_t);
void mmap_prepare_mapping(Mapping *, Arguments);
void mmap_cleanup_mapping(Mapping *);
void mmap_run_benchmark(Mapping *mapping, Arguments args, Results *results);
void mmap_prepare_map_anon(Mapping *mapping, int fsize);
void mmap_init_file(int, int, int);
void mmap_init_mem(Mapping *);
#endif