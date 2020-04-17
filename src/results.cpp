#include <iostream>
#include <fstream>
#include "results.h"

void dump_results(Results results)
{
    std::ofstream outfile;
    outfile.open("results.out");
    outfile << "\t\t==== SUMMARY ====\n";
    outfile << "Runtime \t\t\t\t" << results.runtime << "sec\n";
    outfile << "Sequential Read\t\t\t" << results.seq_read << "MiB/s\n";
    outfile << "Sequential Write\t\t" << results.seq_write << "MiB/s\n";

    outfile.close();
}