#include "parser.h"
#include <iostream>

//TODO Parse cmd line args give all default values
void parse_args(Arguments &args)
{
    args.runtime = 5;
    args.fsize = 268435456; // file size needs to be multiple of block size for alignment and limited to 1GB
    args.bsize = 4096;
    //args.path = "/mnt/mem/benchmark"; // For DAX-mmap and Raw pmem using mounted fs
    args.path = "file"; // For regular mmap any name works, will create if doesn't exist
    args.mode = 0;
    args.raw_pmem = 0; // For Raw pmem access
    args.raw_mem = 0;  // For MAP_ANONYMOUS

    if (args.fsize % args.bsize != 0)
    {
        perror("Not aligned sizes");
        exit(1);
    }
    if (args.raw_pmem && args.raw_mem)
    {
        perror("Can't be both");
        exit(1);
    }
}