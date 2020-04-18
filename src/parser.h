#ifndef PARSR_H
#define PARSER_H

struct Arguments
{
    int runtime;
    const char *path;
    int pmem_len;
    int mode; //0=read 1=write 2=randread 3=randwrite
    int is_dax;
};

void parse_args(Arguments &);

#endif