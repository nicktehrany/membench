#include "functions.h"

// Calculates bandwidth using the counter, runtime, and block size
void get_bandwidth(uint64_t counter, long double runtime, uint64_t bsize, Results *results)
{
    results->bandwidth = (double)counter / runtime * (double)bsize / 1024.0 / 1024.0;
    results->io_data = (double)counter * (double)bsize / 1024.0 / 1024.0 / 1024.0;
}