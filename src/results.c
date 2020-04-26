#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "results.h"

// Summarize all Arguments and Results in a file
void dump_results(Results results, Arguments args)
{
    FILE *fd = fopen("results.out", "w+");
    fprintf(fd, "\t\t==== SUMMARY ====\n");
    fprintf(fd, "Runtime\t\t\t\t\t%ld sec\n", args.runtime);
    if (strcmp(args.path, "file") == 0)
        fprintf(fd, "Directory\t\t\t\tCurrent Directory\n");
    else
        fprintf(fd, "Directory\t\t\t\t%s\n", args.path);

    switch (args.engine)
    {
    case 0:
        fprintf(fd, "Engine\t\t\t\t\tmmap\n");
        break;
    case 1:
        fprintf(fd, "Engine\t\t\t\t\tpmem\n");
        break;
    }

    switch (args.map_anon)
    {
    case 1:
        fprintf(fd, "MAP_ANONYMOUS\t\t\tYes\n");
        break;
    default:
        fprintf(fd, "MAP_ANONYMOUS\t\t\tNo\n");
        break;
    }
    if (args.fsize >= (1024 * 1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld GiB\n", args.fsize / (1024 * 1024 * 1024));
    else if (args.fsize >= (1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld MiB\n", args.fsize / (1024 * 1024));
    else if (args.fsize >= 1024)
        fprintf(fd, "File Size\t\t\t\t%ld KiB\n", args.fsize / 1024);
    else
        fprintf(fd, "File Size\t\t\t\t%ld Bytes\n", args.fsize);

    if (args.buflen >= (1024 * 1024 * 1024))
        fprintf(fd, "Block Size\t\t\t\t%ld GiB\n", args.buflen / ((1024 * 1024 * 1024)));
    else if (args.buflen >= (1024 * 1024))
        fprintf(fd, "Block Size\t\t\t\t%ld MiB\n", args.buflen / ((1024 * 1024)));
    else if (args.buflen >= 1024)
        fprintf(fd, "Block Size\t\t\t\t%ld KiB\n", args.buflen / 1024);
    else
        fprintf(fd, "Block Size\t\t\t\t%ld Bytes\n", args.buflen);

    switch (args.mode)
    {
    case 0:
        if (results.bandwidth >= 1024)
            fprintf(fd, "Sequential Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        else
            fprintf(fd, "Sequential Read\t\t\t%f MiB/s\n", results.bandwidth);
        break;
    case 1:
        if (results.bandwidth >= 1024)
            fprintf(fd, "Sequential Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        else
            fprintf(fd, "Sequential Write\t\t\t%f MiB/s\n", results.bandwidth);
        break;
    case 2:
        if (results.bandwidth >= 1024)
            fprintf(fd, "Random Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        else
            fprintf(fd, "Random Read\t\t\t%f MiB/s\n", results.bandwidth);
        break;
    case 3:
        if (results.bandwidth >= 1024)
            fprintf(fd, "Random Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        else
            fprintf(fd, "Random Write\t\t\t%f MiB/s\n", results.bandwidth);
        break;
    }

    fprintf(fd, "I/O Data\t\t\t\t%f GiB\n", results.io_data);
    fclose(fd);
}
