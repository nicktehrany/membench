#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"

typedef struct Results
{
    double bandwidth;
    double io_data;
} Results;

void dump_results(Results, Arguments);

#endif
