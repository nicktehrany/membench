#include <iostream>
#include <fstream>
#include <string.h>
#include "results.h"

// Summarize all Arguments and Results in a file
void dump_results(Results results, Arguments args)
{
    std::ofstream outfile;
    outfile.open("results.out");
    outfile << "\t\t==== SUMMARY ====\n";
    outfile << "Runtime\t\t\t\t\t" << args.runtime << " sec\n";
    if (strcmp(args.path, "file") == 0)
        outfile << "Directory\t\t\t\tCurrent Directory\n";
    else
        outfile << "Directory\t\t\t\t" << args.path << "\n";

    switch (args.engine)
    {
    case 0:
        outfile << "Engine\t\t\t\t\tmmap\n";
        break;
    case 1:
        outfile << "Engine\t\t\t\t\tpmem\n";
        break;
    }

    switch (args.map_anon)
    {
    case 1:
        outfile << "Map_ANONYMOUS\t\t\tYes\n";
        break;
    default:
        outfile << "Map_ANONYMOUS\t\t\tNo\n";
        break;
    }

    outfile << "File Size\t\t\t\t" << args.fsize << " Bytes\n";
    outfile << "Block Size\t\t\t\t" << args.buflen << " Bytes\n";

    switch (args.mode)
    {
    case 0:
        outfile << "Sequential Read\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 1:
        outfile << "Sequential Write\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 2:
        outfile << "Random Read\t\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 3:
        outfile << "Random Write\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    }
    outfile << "I/O Data\t\t\t\t" << results.io_data << " GiB\n";

    outfile.close();
}