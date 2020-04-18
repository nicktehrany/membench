#include <iostream>
#include <fstream>
#include "results.h"

void dump_results(Results results, Arguments args)
{
    std::ofstream outfile;
    outfile.open("results.out");
    outfile << "\t\t==== SUMMARY ====\n";
    outfile << "Runtime \t\t\t\t" << args.runtime << " sec\n";
    outfile << "Directory \t\t\t\t" << args.path << "\n";
    outfile << "Pmem Size \t\t\t\t" << args.pmem_len << "\n";

    switch (args.mode)
    {
    case 0:
        outfile << "Sequential Read\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 1:
        outfile << "Sequential Write\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 2:
        outfile << "Random Read\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 3:
        outfile << "Random Write\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    }

    outfile.close();
}