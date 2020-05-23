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
#include "results.h"

typedef struct Mapping
{
    char *addr;
    int map_anon; // MAP_ANONYMOUS (not backed by file)
    uint64_t fsize;
    uint64_t buflen;
    const char *fpath;
} Mapping;

void get_bandwidth(uint64_t, double, uint64_t, Results *);
void add_latency(double latency, Results *results);
int set_flags(Arguments);

#endif
