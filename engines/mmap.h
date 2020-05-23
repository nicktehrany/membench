#ifndef MMAP_H
#define MMAP_H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_engine(Arguments *args);
void mmap_check_args(Arguments *args);
void mmap_seq_read(Mapping *mapping, Results *results, Arguments *args);
void mmap_rand_read(Mapping *mapping, Results *results, Arguments *args);
void mmap_seq_write(Mapping *mapping, Results *results, Arguments *args);
void mmap_rand_write(Mapping *mapping, Results *results, Arguments *args);
void mmap_prepare_mapping(Mapping *mapping, Arguments args);
void mmap_cleanup_mapping(Mapping *mapping);
void mmap_run_benchmark(Mapping *mapping, Arguments *args, Results *results);
void mmap_prepare_map_anon(Mapping *mapping, Arguments args);
#endif