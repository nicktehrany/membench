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

    // Depending on engine run things
    mmap_engine(mapping, args, results);

    return 0;
}