#include "results.h"
#include "functions.h"

#include <iostream>

int main()
{
    Results results;
    Mapping mapping;
    results.seq_read = 1;
    results.runtime = 10;
    prepare_mapping(mapping);
    seq_read(mapping, results);
    dump_results(results);
    std::cout << mapping.pmem_addr << " " << mapping.is_pmem << std::endl;

    return 0;
}