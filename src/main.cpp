#include "results.h"
#include "functions.h"
#include <iostream>

int main()
{
    Results results;
    Mapping mapping;
    Arguments args;

    //TODO Implement Parsing args
    parse_args(args);

    prepare_mapping(mapping, args.path, args.fsize, args.raw_pmem);
    mapping.bsize = args.bsize;
    run_benchmark(mapping, args, results);
    cleanup_mapping(mapping);
    dump_results(results, args);

    return 0;
}