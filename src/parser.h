#ifndef PARSR_H
#define PARSER_H

struct Arguments
{
    int runtime;
    const char *path;
    int len;
    int mode;     //0=read 1=write 2=randread 3=randwrite
    int raw_pmem; // Used to check to map raw persistent memory
};

void parse_args(Arguments &);

#endif