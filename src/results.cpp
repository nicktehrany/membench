#include <iostream>
#include <fstream>
#include "results.h"

void dump_results(Results results, Arguments args)
{
    std::ofstream outfile;
    outfile.open("results.out");
    outfile << "\t\t==== SUMMARY ====\n";
    outfile << "Runtime \t\t\t\t" << args.runtime << " sec\n";
    outfile << "Directory \t\t\t\t" << args.dir << "\n";
    outfile << "Pmem Size \t\t\t\t" << args.pmem_len << "\n";
    outfile << "Sequential Read\t\t\t" << results.seq_read << " MiB/s\n";
    outfile << "Sequential Write\t\t" << results.seq_write << " MiB/s\n";

    outfile.close();
}