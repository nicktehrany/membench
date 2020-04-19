#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"

struct Results
{
    double bandwidth;
    double io_data;
};

void dump_results(Results, Arguments);

#endif
