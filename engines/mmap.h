#ifndef H
#define H
#include "../src/results.h"
#include "../src/functions.h"

class Eng_mmap
{
public:
    void mmap_engine(Mapping &, Arguments, Results &);

private:
    void check_args(Arguments &);
    void seq_read(Mapping, Results &, int);
    void rand_read(Mapping, Results &, int);
    void seq_write(Mapping, Results &, int);
    void rand_write(Mapping, Results &, int);
    void prepare_mapping(Mapping &, Arguments);
    void cleanup_mapping(Mapping);
    void run_benchmark(Mapping, Arguments, Results &);
    void prepare_map_anon(Mapping &, int);
    void init_file(int, int, int);
    void init_mem(Mapping);
};
#endif