#include "results.h"
#include "functions.h"

int main()
{
    Results results;
    results.seq_read = 1;
    results.runtime = 10;
    seq_read(results);
    dump_results(results);

    return 0;
}