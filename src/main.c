#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "../engines/mmap_lat.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    Results results = {0, 0};
    Mapping mapping = {0, 0, 0, 0, 0, ""};
    Arguments args = {0, "", 0, 0, 0, 0, 0};

    parse(&args, argc, argv);
    if (args.engine == 0)
        mmap_engine(&mapping, &args, &results);
    else if (args.engine == 1)
        mmap_lat_engine(&mapping, &args, &results);

    return 0;
}