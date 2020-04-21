#include "results.h"
#include "../engines/mmap.h"
#include <iostream>

int main()
{
    Results results;
    Mapping mapping;
    Arguments args;

    //TODO Implement Parsing args
    parse_args(args);

    // Depending on engine run things
    prepare_mapping(mapping, args);
    run_benchmark(mapping, args, results);
    cleanup_mapping(mapping);
    dump_results(results, args);

    return 0;
}