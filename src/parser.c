#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void parse(Arguments *args, int argc, char **argv)
{

    char *tokens[100]; // Max 100 tokens in file or cmd line
    int file_parse = 0;

    if (argc > 100)
        LOG(ERROR, EINVAL, "Too many commands");
    for (int i = 0; i < argc - 1; i++)
    {
        if (strncmp(argv[i + 1], "-h", 2) == 0)
            display_help();
        else if (strncmp(argv[i + 1], "-file=", 5) == 0)
        {
            argc = parse_file(argv[i + 1], tokens);
            file_parse = 1;
            break;
        }
        tokens[i] = argv[i + 1];
    }
    parse_cmd_line(args, tokens, argc - 1);
    if (file_parse)
    {
        for (int i = 0; i < argc - 1; i++)
            free_tok(tokens[i]); // Freeing all mallocs from file parsing
    }
}

void parse_cmd_line(Arguments *args, char *tokens[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strncmp(tokens[i], "-runtime=", 9) == 0)
            set_runtime(tokens[i], args);
        else if (strncmp(tokens[i], "-size=", 6) == 0)
            set_size(tokens[i], args);
        else if (strncmp(tokens[i], "-copysize=", 10) == 0)
            set_buflen(tokens[i], args);
        else if (strncmp(tokens[i], "-dir=", 5) == 0)
            set_path(tokens[i], args);
        else if (strncmp(tokens[i], "-mode=", 6) == 0)
            set_mode(tokens[i], args);
        else if (strncmp(tokens[i], "-engine=", 8) == 0)
            set_engine(tokens[i], args);
        else if (strncmp(tokens[i], "-iter=", 6) == 0)
            set_iter(tokens[i], args);
        else if (strncmp(tokens[i], "-map_pop=", 9) == 0)
            set_map_pop(tokens[i], args);
        else if (strncmp(tokens[i], "-map_shared=", 12) == 0)
            set_map_shared(tokens[i], args);
        else if (strncmp(tokens[i], "-cpy_iter=", 10) == 0)
            set_cpy_iter(tokens[i], args);
        else if (strncmp(tokens[i], "#", 1) == 0)
            ;
        else
        {
            char buf[256];
            snprintf(buf, sizeof(buf), "Unknow command %s", tokens[i]);
            LOG(ERROR, EINVAL, buf);
        }
    }
}

int parse_file(char *token, char *tokens[])
{
    char *temp = token;
    FILE *fp = fopen(temp + 6, "r+");

    if (fp == NULL)
    {
        errno = EINVAL;
        perror("Invalid file");
        exit(1);
    }

    char *line = "";
    int counter = 0;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1)
    {
        if (counter > 100)
            LOG(ERROR, EINVAL, "Too many commands");
        char *temp = calloc(strlen(line), 1);
        size_t len = strlen(line) - 1;
        memcpy(temp, line, len);
        tokens[counter] = temp;
        counter++;
    }
    free_tok(line); // Freeing malloc from getline()
    fclose(fp);
    return counter + 1; // Account for program call in cmd line (./Benchmark)
}

void display_help()
{
    printf("Possible commands are:\n");
    printf("-engine=\tPossible engines are mmap, mmap_lat, mem_lat, page_fault\n");
    printf("-file=\t\tProvide an input file with commands as shown in examples\n");
    printf("-runtime=\tSet runtime seconds\n");
    printf("-size=\t\tSet file size (For example 2M for 2MiB file)\n");
    printf("-copysize=\tSet copy size for memcpy (For example 4K for 4KiB)\n");
    printf("-cpy_iter=\tNumber of times to call memcpy for mmap_eng\n");
    printf("-dir=\t\tPath to directory to use (/dev/null or /dev/zero for MAP_ANONYMOUS)\n");
    printf("-mode=\t\tPossible modes are: read write randread randwrite (Default read)\n");
    printf("-iter=\t\tNumber of times to run engine (Default 1\n");
    printf("-map_pop=\t0|1 to pass MAP_POPULATE to mmap for mmap_lat engine (Default 0)\n");
    printf("-map_shared=\t0|1 to specify to pass MAP_SHARED or MAP_PRIVATE to mmap (Default 0)\n");
    printf("\nFor usage of engine specific commands consult the documentation\n");
    printf("Commands invalid for engine will be disregarded\n");
    exit(0);
}

void set_runtime(char *token, Arguments *args)
{
    char *temp = (char *)token;
    args->runtime = atoi(temp + 9);
    if (args->runtime <= 0)
        LOG(ERROR, EINVAL, "Invalid runtime");
}

void set_size(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;
    int multiplier = 1;

    if (*unit == 'K')
        multiplier = 1024;
    else if (*unit == 'M')
        multiplier = 1024 * 1024;
    else if (*unit == 'G')
        multiplier = 1024 * 1024 * 1024;
    else if (*unit != 'B')
        LOG(ERROR, EINVAL, "Invalid Size");

    char *ptr;
    args->size = strtoul(temp + 6, &ptr, 10) * multiplier;
}

void set_buflen(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;

    int multiplier = 1;

    if (*unit == 'K')
        multiplier = 1024;
    else if (*unit == 'M')
        multiplier = 1024 * 1024;
    else if (*unit == 'G')
        multiplier = 1024 * 1024 * 1024;
    else if (*unit != 'B')
        LOG(ERROR, EINVAL, "Invalid Size");

    char *ptr;
    args->buflen = strtoul(temp + 10, &ptr, 10) * multiplier;
    if (args->buflen <= 0)
        LOG(ERROR, EINVAL, "Invalid copy size");
}

void set_path(char *token, Arguments *args)
{
    char *temp = token;
    char *dir = (char *)malloc(strlen(temp) + 1);
    strcpy(dir, temp + 5);
    if (strcmp(dir, "/dev/null") == 0 || strcmp(dir, "/dev/zero") == 0)
        args->map_anon = 1;
    if (!(args->path = malloc(strlen(dir) + 1)))
        LOG(ERROR, EINVAL, "Invalid Directory");
    strcpy(args->path, dir);
    free_tok(dir);
}

void set_mode(char *token, Arguments *args)
{
    char *temp = token;
    char *mode = temp + 6;
    if (strncmp(mode, "read", 4) == 0)
        args->mode = 0;
    else if (strncmp(mode, "write", 5) == 0)
        args->mode = 1;
    else if (strncmp(mode, "randread", 8) == 0)
        args->mode = 2;
    else if (strncmp(mode, "randwrite", 9) == 0)
        args->mode = 3;
    else
        LOG(ERROR, EINVAL, "Invalid mode");
}

void set_engine(char *token, Arguments *args)
{
    char *temp = token;
    char *mode = temp + 8;
    if (strncmp(mode, "mmap_lat", 8) == 0)
        args->engine = 1;
    else if (strncmp(mode, "mmap", 4) == 0)
        args->engine = 0;
    else if (strncmp(mode, "mem_lat", 7) == 0)
        args->engine = 2;
    else if (strncmp(mode, "page_fault", 10) == 0)
        args->engine = 3;
    else if (strncmp(mode, "pmem_cline", 10) == 0)
        args->engine = 4;
}

void set_iter(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;
    int multiplier = 1;
    char K = 'K', M = 'M';
    if (*unit == K)
        multiplier = 1000;
    else if (*unit == M)
        multiplier = 1000 * 1000;

    char *ptr;
    args->iterations = strtoul(temp + 6, &ptr, 10) * multiplier;
    if (args->iterations < 1)
    {
        LOG(INFO, EINVAL, "Invalid Iterations. Running default 1 iteration");
        args->iterations = 1;
    }
}

void set_map_pop(char *token, Arguments *args)
{
    char *temp = token;
    char *ptr;
    args->map_pop = strtoul(temp + 9, &ptr, 10);
    if (args->map_pop > 1 || args->map_pop < 0)
    {
        errno = EINVAL;
        LOG(INFO, EINVAL, "Invalid value for map_pop. Running default 0");
        args->map_pop = 0;
    }
}

void set_map_shared(char *token, Arguments *args)
{
    char *temp = token;
    char *ptr;
    args->map_shared = strtoul(temp + 12, &ptr, 10);
    if (args->map_shared > 1 || args->map_shared < 0)
    {
        LOG(INFO, EINVAL, "Invalid value for map_shared. Running default 0");
        args->map_shared = 0;
    }
}

void set_cpy_iter(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;
    int multiplier = 1;
    char K = 'K', M = 'M';
    if (*unit == K)
        multiplier = 1000;
    else if (*unit == M)
        multiplier = 1000 * 1000;

    char *ptr;
    args->cpy_iter = strtoul(temp + 10, &ptr, 10) * multiplier;
    if (args->cpy_iter < 1)
    {
        LOG(INFO, EINVAL, "Invalid cpy iterations. Running default 1 iteration");
        args->cpy_iter = 1;
    }
}

void free_tok(char *token)
{
    free(token);
    token = NULL;
}