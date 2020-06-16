#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/mmap_lat.h"
#include "../engines/mem_lat.h"
#include "../engines/page_fault.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    Arguments args = {0, "", 0, 0, 0, 0, -1, 1, 0, 0, 1};

    parse(&args, argc, argv);
    switch (args.engine)
    {
    case 0:
        mmap_engine(&args);
        break;
    case 1:
        mmap_lat_engine(&args);
        break;
    case 2:
        mem_lat_engine(&args);
        break;
    case 3:
        page_fault_lat_engine(&args);
        break;
    default:
        LOG(ERROR, EINVAL, "Engine");
        break;
    }

    if (strlen(args.path) > 0)
        free(args.path);

    return 0;
}