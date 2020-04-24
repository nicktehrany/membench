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
        outfile << "MAP_ANONYMOUS\t\t\tYes\n";
        break;
    default:
        outfile << "MAP_ANONYMOUS\t\t\tNo\n";
        break;
    }
    if (args.fsize > (1024 * 1024 * 1024))
        outfile << "File Size\t\t\t\t" << args.fsize / (1024 * 1024 * 1024) << " GiB\n";
    else if (args.fsize > (1024 * 1024))
        outfile << "File Size\t\t\t\t" << args.fsize / (1024 * 1024) << " MiB\n";
    else if (args.fsize > 1024)
        outfile << "File Size\t\t\t\t" << args.fsize / 1024 << " KiB\n";
    else
        outfile << "File Size\t\t\t\t" << args.fsize << " Bytes\n";

    if (args.buflen > (1024 * 1024 * 1024))
        outfile << "Block Size\t\t\t\t" << args.buflen / ((1024 * 1024 * 1024)) << " GiB\n";
    else if (args.buflen > (1024 * 1024))
        outfile << "Block Size\t\t\t\t" << args.buflen / ((1024 * 1024)) << " MiB\n";
    else if (args.buflen > 1024)
        outfile << "Block Size\t\t\t\t" << args.buflen / 1024 << " KiB\n";
    else
        outfile << "Block Size\t\t\t\t" << args.buflen << " Bytes\n";

    switch (args.mode)
    {
    case 0:
        if (results.bandwidth > 1024)
            outfile << "Sequential Read\t\t\t" << results.bandwidth / 1024 << " GiB/s\n";
        else
            outfile << "Sequential Read\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 1:
        if (results.bandwidth > 1024)
            outfile << "Sequential Write\t\t\t" << results.bandwidth / 1024 << " GiB/s\n";
        else
            outfile << "Sequential Write\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 2:
        if (results.bandwidth > 1024)
            outfile << "Random Read\t\t\t" << results.bandwidth / 1024 << " GiB/s\n";
        else
            outfile << "Random Read\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    case 3:
        if (results.bandwidth > 1024)
            outfile << "Random Write\t\t\t" << results.bandwidth / 1024 << " GiB/s\n";
        else
            outfile << "Random Write\t\t\t" << results.bandwidth << " MiB/s\n";
        break;
    }

    outfile << "I/O Data\t\t\t\t" << results.io_data << " GiB\n";

    outfile.close();
}