#ifndef PMEM_CLINE_H
#define PMEM_CLINE_H
#include "../src/parser.h"
#include "../src/functions.h"
#include <libpmem.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Pmem_Map
{
    char *addr;
    size_t len;
    int is_pmem;
} Pmem_Map;

void pmem_cline_engine(Arguments *args);
void pmem_cline_prepare_mapping(Arguments *args, Pmem_Map *pmem_map);
void pmem_cline_benchmark(Arguments *args, Pmem_Map *pmem_map);
uint64_t pmem_cline_flush_chunck(Pmem_Map *pmem_map);
uint64_t pmem_cline_flush_lines(Pmem_Map *pmem_map);

#endif