#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

// Default values
typedef struct Arguments
{
    double runtime;
    char *path;
    uint64_t fsize;
    uint64_t buflen;
    int mode;     //0=read 1=write 2=randread 3=randwrite
    int map_anon; // Mapping without backing of file on file system
    int engine;   // Default mmap engine
    uint64_t iterations;
    int map_pop;
    int map_shared;
} Arguments;

void parse(Arguments *, int, char **);
void display_help();
void parse_cmd_line(Arguments *, char *[], int);
int parse_file(char *, char *[]);
void set_runtime(char *, Arguments *);
void set_filesize(char *, Arguments *);
void set_buflen(char *, Arguments *);
void set_path(char *, Arguments *);
void set_mode(char *, Arguments *);
void set_engine(char *, Arguments *);
void set_iter(char *token, Arguments *args);
void set_map_pop(char *token, Arguments *args);
void set_map_shared(char *token, Arguments *args);

#endif