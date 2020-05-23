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

void dump_results(Results, Arguments);
void results_mmap_eng(Arguments args, FILE *fd, Results results);
void results_mmap_lat_eng(FILE *fd, Results results);
void results_mem_lat_eng(FILE *fd, Results results, Arguments);
void display_results(FILE *);

#endif
