#include "parser.h"

//TODO Parse cmd line args
void parse_args(Arguments &args)
{
    args.runtime = 5;
    args.len = 4096;
    args.path = "/mnt/mem/benchmark";
    //args.path = "file";
    args.mode = 1;
    args.raw_pmem = 0;
}