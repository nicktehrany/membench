#ifndef PMEM_H
#define PMEM_H
#include "../src/results.h"
#include "../src/functions.h"

void pmem_engine(Mapping &, Arguments, Results &);
void pmem_seq_read(Mapping, Results &, int);
void pmem_rand_read(Mapping, Results &, int);
void pmem_seq_write(Mapping, Results &, int);
void pmem_rand_write(Mapping, Results &, int);
void pmem_prepare_mapping(Mapping &, Arguments);
void pmem_cleanup_mapping(Mapping);
void pmem_run_benchmark(Mapping, Arguments, Results &);
#endif