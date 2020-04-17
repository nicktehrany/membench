#include "results.h"
#include "functions.h"

#include <iostream>

int main()
{
    Results results;
    Mapping mapping;

    //TODO Take as parameter
    results.runtime = 10;
    prepare_mapping(mapping);
    seq_read(mapping, results);
    seq_write(mapping, results);
    dump_results(results);

    return 0;
}