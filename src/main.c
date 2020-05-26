#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/mmap_lat.h"
#include "../engines/mem_lat.h"
#include "../engines/page_fault.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    Arguments args = {0, "", 0, 0, 0, 0, -1, 1, 0, 0, 0};

    parse(&args, argc, argv);
    if (args.engine == 0)
        mmap_engine(&args);
    else if (args.engine == 1)
        mmap_lat_engine(&args);
    else if (args.engine == 2)
        mem_lat_engine(&args);
    else if (args.engine == 3)
        page_fault_lat_engine(&args);

    if (strlen(args.path) > 0)
        free(args.path);

    return 0;
}