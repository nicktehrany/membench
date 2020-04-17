#include "functions.h"
#include <libpmem.h>
#define	PMEM_LEN 4096

void seq_read(Results &results)
{
    results.seq_read = 100;
}