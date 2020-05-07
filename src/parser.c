#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void parse(Arguments *args, int argc, char **argv)
{

    char *tokens[100]; // Max 100 tokens in file or cmd line

    if (argc > 100)
    {
        errno = EINVAL;
        perror("Too many commands");
        exit(1);
    }
    for (int i = 0; i < argc - 1; i++)
    {
        if (strncmp(argv[i + 1], "-h", 2) == 0)
            display_help();
        else if (strncmp(argv[i + 1], "-file=", 5) == 0)
        {
            argc = parse_file(argv[i + 1], tokens);
            break;
        }
        tokens[i] = argv[i + 1];
    }
    parse_cmd_line(args, tokens, argc - 1);
}

void parse_cmd_line(Arguments *args, char *tokens[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strncmp(tokens[i], "-runtime=", 9) == 0)
            set_runtime(tokens[i], args);
        else if (strncmp(tokens[i], "-fsize=", 7) == 0)
            set_filesize(tokens[i], args);
        else if (strncmp(tokens[i], "-copysize=", 10) == 0)
            set_buflen(tokens[i], args);
        else if (strncmp(tokens[i], "-dir=", 5) == 0)
            set_path(tokens[i], args);
        else if (strncmp(tokens[i], "-mode=", 6) == 0)
            set_mode(tokens[i], args);
        else if (strncmp(tokens[i], "-engine=", 8) == 0)
            set_engine(tokens[i], args);
        else
        {
            printf("Unknow command %s\n", tokens[i]);
            exit(1);
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
        {
            errno = EINVAL;
            perror("Too many commands");
            exit(1);
        }
        char *x = calloc(strlen(line), 1);
        size_t len = strlen(line) - 1;
        memcpy(x, line, len);
        tokens[counter] = x;
        counter++;
    }
    fclose(fp);
    return counter + 1; // Account for program call in cmd line
}

void display_help()
{
    printf("Possible commands are:\n");
    printf("-file=\t\tProvide an input file with commands as shown in examples\n");
    printf("-runtime=\tSet runtime seconds\n");
    printf("-fsize=\tSet file size (For example 2M for 2MiB file)\n");
    printf("-copysize=\tSet copy size for memcpy (For example 4K for 4KiB)\n");
    printf("-dir=\t\tPath to directory to use (/dev/null or /dev/zero for MAP_ANONYMOUS, current if none specified)\n");
    printf("-mode=\t\tPossible modes are: read write randread randwrite (Default read)\n");
    printf("-engine=\tPossible engines are mmap and pmem (Default mmap)\n");
    exit(0);
}

void set_runtime(char *token, Arguments *args)
{
    char *temp = (char *)token;
    args->runtime = atoi(temp + 9);
    if (args->runtime <= 0)
    {
        errno = EINVAL;
        perror("Invalid runtime");
        exit(1);
    }
}

void set_filesize(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;
    int multiplier = 1;
    char K = 'K', M = 'M', G = 'G';
    if (*unit == K)
        multiplier = 1024;
    else if (*unit == M)
        multiplier = 1024 * 1024;

    else if (*unit == G)
        multiplier = 1024 * 1024 * 1024;

    char *ptr;
    args->fsize = strtoul(temp + 7, &ptr, 10) * multiplier;
    if (args->fsize < 4096)
    {
        errno = EINVAL;
        perror("Invalid File size. Needs to be at least 32KiB");
        exit(1);
    }
}

void set_buflen(char *token, Arguments *args)
{
    char *temp = token;

    char *unit = temp + strlen(temp) - 1;

    int multiplier = 1;
    char K = 'K', M = 'M', G = 'G';

    if (*unit == K)
        multiplier = 1024;
    else if (*unit == M)
        multiplier = 1024 * 1024;
    else if (*unit == G)
        multiplier = 1024 * 1024 * 1024;

    char *ptr;
    args->buflen = strtoul(temp + 10, &ptr, 10) * multiplier;
    if (args->buflen <= 0)
    {
        errno = EINVAL;
        perror("Invalid copy size");
        exit(1);
    }
}

void set_path(char *token, Arguments *args)
{
    char *temp = token;
    char *dir = (char *)malloc(strlen(temp) + 1);
    strcpy(dir, temp + 5);
    if (strcmp(dir, "/dev/null") == 0 || strcmp(dir, "/dev/zero") == 0)
        args->map_anon = 1;
    else
    {
        char *slash = temp + strlen(temp) - 1;
        char sl = '/';
        if (*slash == sl)
            dir = strcat(dir, "file");
        else
            dir = strcat(dir, "/file");
    }
    if (!(args->path = malloc(strlen(dir) + 1)))
    {
        errno = EINVAL;
        perror("Invalid Directory.");
    }
    strcpy(args->path, dir);
    free(dir);
    dir = NULL;
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
    {
        errno = EINVAL;
        perror("Invalid mode");
        exit(1);
    }
}

void set_engine(char *token, Arguments *args)
{
    char *temp = token;
    char *mode = temp + 8;
    if (strncmp(mode, "mmap_lat", 8) == 0)
        args->engine = 1;
    else if (strncmp(mode, "mmap", 4) == 0)
        args->engine = 0;
    else
    {
        errno = EINVAL;
        perror("Invalid engine");
        exit(1);
    }
}