#include "functions.h"

// Calculates bandwidth using the counter, runtime, and block size
void get_bandwidth(int counter, int runtime, int bsize, Results &results)
{
    results.bandwidth = (double)counter / (double)runtime * (double)bsize / 1024.0 / 1024.0;
    results.io_data = (double)counter * (double)bsize / 1024.0 / 1024.0 / 1024.0;
}