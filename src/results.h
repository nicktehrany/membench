#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Results
{
    long double bandwidth;
    uint64_t io_data;
    double min_lat;
    double max_lat;
    double avg_lat;
} Results;

typedef struct Size_Unit
{
    long double size;
    char *unit;
} Size_Unit;

void dump_results(Results results, Arguments args);
void results_mmap_eng(FILE *fp, Results results, Arguments args);
void results_mmap_lat_eng(FILE *fp, Results results, Arguments args);
void results_mem_lat_eng(FILE *fp, Results results, Arguments args);
void print_dir(FILE *fp, Arguments args);
void print_flags(FILE *fp, Arguments args);
void print_misc(FILE *fp, Arguments args);
void print_latencies(FILE *fp, Results results);
void results_page_fault_eng(FILE *fp, Results results, Arguments args);
void format_size(Size_Unit *size_unit, uint64_t size);
void format_latency(Size_Unit *size_unit, uint64_t latency);

#endif
