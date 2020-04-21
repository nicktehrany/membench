#ifndef PARSR_H
#define PARSER_H

struct Arguments
{
    int runtime;
    const char *path;
    int fsize;
    int bsize;
    int mode;     //0=read 1=write 2=randread 3=randwrite
    int raw_pmem; // Mapping raw persistent memory
    int raw_mem;  // Mapping without backing of file on file system
};

void parse_args(Arguments &);

#endif