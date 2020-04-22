#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/pmem.h"

int main(int argc, char *argv[])
{
    Results results;
    Mapping mapping;
    Arguments args;

    Parser parser(args, argc, argv);
    if (args.engine == 0)
    {
        Eng_mmap engine;
        engine.mmap_engine(mapping, args, results);
    }
    else if (args.engine == 1)
    {
        Eng_pmem engine;
        engine.pmem_engine(mapping, args, results);
    }

    return 0;
}