#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

// Default values
typedef struct Arguments
{
    double runtime;
    char *path;
    uint64_t size;
    uint64_t buflen;
    int mode;     //0=read 1=write 2=randread 3=randwrite
    int map_anon; // Mapping without backing of file on file system
    int engine;
    uint64_t iterations;
    int map_pop;
    int map_shared;
    uint64_t cpy_iter; // For mmap_eng number of memcpy calls on single mapping
} Arguments;

void parse(Arguments *args, int argc, char **argv);
void display_help();
void parse_cmd_line(Arguments *args, char *tokens[], int size);
int parse_file(char *token, char *tokens[]);
void set_runtime(char *token, Arguments *args);
void set_size(char *token, Arguments *args);
void set_buflen(char *token, Arguments *args);
void set_path(char *token, Arguments *args);
void set_mode(char *token, Arguments *args);
void set_engine(char *token, Arguments *args);
void set_iter(char *token, Arguments *args);
void set_map_pop(char *token, Arguments *args);
void set_map_shared(char *token, Arguments *args);
void set_cpy_iter(char *token, Arguments *args);

#endif