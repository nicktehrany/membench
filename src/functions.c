#include "functions.h"
#include <sys/mman.h>

// Calculates bandwidth using the counter, runtime, and copy size
void get_bandwidth(uint64_t counter, double runtime, uint64_t bsize, Results *results)
{
    results->bandwidth = (double)counter / runtime * (double)bsize;
    results->io_data = (double)counter * (double)bsize;
}

void add_latency(double latency, Results *results)
{
    if (latency > results->max_lat)
        results->max_lat = latency;
    if (latency < results->min_lat || results->min_lat == 0)
        results->min_lat = latency;
}

int set_flags(Arguments args)
{
    int flags = 0;

    if (args.map_pop)
        flags |= MAP_POPULATE;
    if (args.map_anon)
        flags |= MAP_ANONYMOUS;
    if (args.map_shared)
        flags |= MAP_SHARED;
    else
        flags |= MAP_PRIVATE;

    return flags;
}