#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/pmem.h"

int main(int argc, char *argv[])
{
    Results results;
    Mapping mapping;
    Arguments args;

    //TODO Implement Parsing args
    parse_args(args);

    if (args.raw_pmem)
    {
        Eng_pmem engine;
        engine.pmem_engine(mapping, args, results);
    }
    else
    {
        Eng_mmap engine;
        engine.mmap_engine(mapping, args, results);
    }

    return 0;
}