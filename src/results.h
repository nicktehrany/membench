#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"
#include <stdio.h>

typedef struct Results
{
    double bandwidth;
    uint64_t io_data;
    double min_lat;
    double max_lat;
    double avg_lat;
} Results;

void dump_results(Results results, Arguments args);
void results_mmap_eng(FILE *fp, Results results, Arguments args);
void results_mmap_lat_eng(FILE *fp, Results results, Arguments args);
void results_mem_lat_eng(FILE *fp, Results results, Arguments args);
void print_dir(FILE *fp, Arguments args);
void print_flags(FILE *fp, Arguments args);
void print_misc(FILE *fp, Arguments args);
void display_results(FILE *fp);
void results_page_fault_eng(FILE *fp, Results results, Arguments args);

#endif
