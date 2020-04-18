#include <iostream>
#include <fstream>
#include "results.h"

// Summarize all Arguments and Results in a file
void dump_results(Results results, Arguments args)
{
    std::ofstream outfile;
    outfile.open("results.out");
    outfile << "\t\t==== SUMMARY ====\n";
    outfile << "Runtime \t\t\t\t" << args.runtime << " sec\n";
    outfile << "Directory \t\t\t\t" << args.path << "\n";

    switch (args.raw_pmem)
    {
    case 1:
        outfile << "Raw PMEM \t\t\t\tYes\n";
        break;
    default:
        outfile << "Raw PMEM \t\t\t\tNo\n";
        break;
    }

    outfile << "Mem Size \t\t\t\t" << args.len << "Bytes\n";

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