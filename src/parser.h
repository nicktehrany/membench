#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

// Default values
typedef struct Arguments
{
    uint64_t runtime;
    char *path;
    uint64_t fsize;
    uint64_t buflen;
    int mode;     //0=read 1=write 2=randread 3=randwrite
    int map_anon; // Mapping without backing of file on file system
    int engine;   // Default mmap engine
    uint64_t iterations;
} Arguments;

void parse(Arguments *, int, char **);
void check_args(Arguments *);
void display_help();
void parse_cmd_line(Arguments *, char *[], int);
int parse_file(char *, char *[]);
void set_runtime(char *, Arguments *);
void set_filesize(char *, Arguments *);
void set_buflen(char *, Arguments *);
void set_path(char *, Arguments *);
void set_mode(char *, Arguments *);
void set_engine(char *, Arguments *);
#endif