#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/mmap_lat.h"
#include "../engines/mem_lat.h"
#include "parser.h"
#define DEF_CPY_ITER 100000

int main(int argc, char *argv[])
{
    Arguments args = {0, "", 0, 0, 0, 0, -1, 0, 0, 0, (uint64_t)DEF_CPY_ITER};

    parse(&args, argc, argv);
    if (args.engine == 0)
        mmap_engine(&args);
    else if (args.engine == 1)
        mmap_lat_engine(&args);
    else if (args.engine == 2)
        mem_lat_engine(&args);

    return 0;
}