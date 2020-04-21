#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/pmem.h"

int main()
{
    Results results;
    Mapping mapping;
    Arguments args;

    //TODO Implement Parsing args
    parse_args(args);

    if (args.raw_pmem)
        pmem_engine(mapping, args, results);
    else
        mmap_engine(mapping, args, results);

    return 0;
}