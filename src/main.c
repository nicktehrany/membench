#include "results.h"
#include "functions.h"
#include "../engines/mmap.h"
#include "parser.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Results results = {0, 0};
    Mapping mapping = {0, 0, 0, 0, 0, ""};
    Arguments args = {0, "", 0, 0, 0, 0, 0};

    parse(&args, argc, argv);
    if (args.engine == 0)
        mmap_engine(&mapping, &args, &results);
    free(args.path);
    return 0;
}