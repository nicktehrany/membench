#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/mmap_lat.h"
#include "../engines/mem_lat.h"
#include "../engines/page_fault.h"
#include "parser.h"
#ifdef PMEM
#include "../engines/pmem_cline.h"
#endif

int main(int argc, char *argv[])
{
    Arguments args = {0, "", 0, 0, 0, 0, -1, 1, 0, 0, 1};

    parse(&args, argc, argv);
    if (args.engine == 0)
        mmap_engine(&args);
    else if (args.engine == 1)
        mmap_lat_engine(&args);
    else if (args.engine == 2)
        mem_lat_engine(&args);
    else if (args.engine == 3)
        page_fault_lat_engine(&args);
#ifdef PMEM
    else if (args.engine == 4)
        pmem_cline_engine(&args);
#endif
    else
    {
        if (args.engine == 4)
            LOG(ERROR, ENOTSUP, "Running basic version. Run \"make full\" for pmem engine");
        else
            LOG(ERROR, EINVAL, "Engine");
    }

    if (strlen(args.path) > 0)
        free(args.path);

    return 0;
}