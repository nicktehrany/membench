#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define NANS_TO_SECS 1.0e-9
#define SECS_TO_NANS 1.0e9
#define SECS_ELAPSED(tend, tstart) (((double)tend.tv_sec + NANS_TO_SECS * tend.tv_nsec) - \
                                    ((double)tstart.tv_sec + NANS_TO_SECS * tstart.tv_nsec))
#define NANS_ELAPSED(tend, tstart) (((tend.tv_sec - tstart.tv_sec) * SECS_TO_NANS) + (tend.tv_nsec - tstart.tv_nsec))
#define PAGESIZE sysconf(_SC_PAGE_SIZE)
#define FATAL -1
#define ERROR 1
#define LOG(type, error_val, message) \
    (type == FATAL) ? (errno = error_val, perror(message), exit(1)) : (void)printf("%s\n", message)
#include "results.h"

typedef struct Mapping
{
    char *addr;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    uint64_t size;
    uint64_t buflen;
    const char *fpath;
} Mapping;

void get_bandwidth(uint64_t counter, double runtime, uint64_t bsize, Results *results);
void add_latency(double latency, Results *results);
int set_flags(Arguments args);

#endif
