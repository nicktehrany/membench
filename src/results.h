#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"
#include <stdio.h>

typedef struct Results
{
    double bandwidth;
    double io_data;
    double min_lat;
    double max_lat;
    double avg_lat;
} Results;

void dump_results(Results results, Arguments args);
void results_mmap_eng(FILE *fd, Results results, Arguments args);
void results_mmap_lat_eng(FILE *fd, Results results, Arguments args);
void results_mem_lat_eng(FILE *fd, Results results, Arguments args);
void print_dir(FILE *fd, Arguments args);
void print_flags(FILE *fd, Arguments args);
void print_misc(FILE *fd, Arguments args);
void display_results(FILE *fd);

#endif
