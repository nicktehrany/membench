#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "results.h"

struct Mapping
{
    int *pmem_addr;
    int is_pmem;
    int pmem_len;
};

void seq_read(Mapping, Results &, int);
void seq_write(Mapping, Results &);
void prepare_mapping(Mapping &, const char *, int);
void initialize_pmem(Mapping);
void cleanup_mapping(Mapping);

#endif