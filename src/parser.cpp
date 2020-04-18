#include "parser.h"

//TODO Parse cmd line args
void parse_args(Arguments &args)
{
    args.runtime = 60;
    args.pmem_len = 4096;
    args.path = "/mnt/mem/benchmark";
    args.mode = 1;
    args.is_dax = 0;
}