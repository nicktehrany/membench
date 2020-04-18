#ifndef PARSR_H
#define PARSER_H

struct Arguments
{
    int runtime;
    const char *dir;
    int pmem_len;
};

void parse_args(Arguments &);

#endif