#ifndef RESULTS_H
#define RESULTS_H

struct Results
{
    double seq_read;
    double seq_write;
    int runtime;
};

void dump_results(Results);

#endif
