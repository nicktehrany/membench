#include "functions.h"
#include <sys/mman.h>

// Calculates bandwidth using the counter, runtime, and block size
void get_bandwidth(uint64_t counter, long double runtime, uint64_t bsize, Results *results)
{
    results->bandwidth = (double)counter / runtime * (double)bsize / 1024.0 / 1024.0;
    results->io_data = (double)counter * (double)bsize / 1024.0 / 1024.0 / 1024.0;
}

void add_latency(uint64_t latency, Results *results)
{
    if (latency > results->max_lat)
        results->max_lat = latency;
    if (latency < results->min_lat || results->min_lat == 0)
        results->min_lat = latency;
}

int set_flags(Arguments args)
{
    int flags;
    if (args.map_pop)
    {
        if (args.map_shared)
            flags = MAP_SHARED | MAP_POPULATE;
        else
            flags = MAP_PRIVATE | MAP_POPULATE;
    }
    else if (args.map_shared)
        flags = MAP_SHARED;
    else
        flags = MAP_PRIVATE;

    return flags;
}