#ifndef PMEM_H
#define PMEM_H
#include "../src/results.h"
#include "../src/functions.h"

class Eng_pmem
{
public:
    void pmem_engine(Mapping &, Arguments, Results &);

private:
    void check_args(Arguments &);
    void seq_read(Mapping, Results &, int);
    void rand_read(Mapping, Results &, int);
    void seq_write(Mapping, Results &, int);
    void rand_write(Mapping, Results &, int);
    void prepare_mapping(Mapping &, Arguments);
    void cleanup_mapping(Mapping);
    void run_benchmark(Mapping, Arguments, Results &);
};

#endif