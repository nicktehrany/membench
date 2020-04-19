#include "parser.h"

//TODO Parse cmd line args
void parse_args(Arguments &args)
{
    args.runtime = 60;
    args.fsize = 4096 * 10000; // file size needs to be multiple of block size for alignment and limited to 1GB
    args.bsize = 4096;
    //args.path = "/mnt/mem/benchmark"; // For DAX-mmap and Raw pmem using mounted fs
    args.path = "file"; // For regular mmap
    args.mode = 0;
    args.raw_pmem = 0; // For Raw pmem access
}