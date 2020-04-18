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

    prepare_mapping(mapping, args.dir, args.pmem_len);
    seq_read(mapping, results, args.runtime);
    seq_write(mapping, results);
    cleanup_mapping(mapping);
    dump_results(results, args);

    return 0;
}