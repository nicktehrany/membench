#include "parser.h"
#include <iostream>

//TODO Parse cmd line args
void parse_args(Arguments &args)
{
    args.runtime = 2;
    args.fsize = 41943040; // file size needs to be multiple of block size for alignment and limited to 1GB
    args.bsize = 4096;
    //args.path = "/mnt/mem/benchmark"; // For DAX-mmap and Raw pmem using mounted fs
    args.path = "file"; // For regular mmap any name works will create if doesn't exist
    args.mode = 0;
    args.raw_pmem = 0; // For Raw pmem access

    if (args.fsize % args.bsize != 0)
    {
        perror("Not aligned sizes");
        exit(1);
    }
}