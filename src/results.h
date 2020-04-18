#ifndef RESULTS_H
#define RESULTS_H
#include "parser.h"

struct Results
{
    double seq_read;
    double seq_write;
};

void dump_results(Results, Arguments);

#endif
