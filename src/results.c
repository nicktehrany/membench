#include <stdlib.h>
#include <string.h>
#include "results.h"

// Summarize all Arguments and Results in a file
void dump_results(Results results, Arguments args)
{
    FILE *fd = fopen("results.out", "w+");
    if (fd == NULL)
    {
        perror("Failed opening results.out file. Delete it or run as sudo!");
        exit(1);
    }
    fprintf(fd, "\t\t==== SUMMARY ====\n");
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
        fprintf(fd, "Engine\t\t\t\t\tmmap_lat\n");
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

    if (args.engine == 0)
        results_mmap_eng(args, fd, results);
    else if (args.engine == 1)
        results_mmap_lat_eng(args, fd, results);
    fclose(fd);
}

void results_mmap_eng(Arguments args, FILE *fd, Results results)
{
    fprintf(fd, "Runtime\t\t\t\t\t%ld sec\n", args.runtime);
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
        fprintf(fd, "Sequential Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 1:
        fprintf(fd, "Sequential Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 2:
        fprintf(fd, "Random Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 3:
        fprintf(fd, "Random Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    }

    fprintf(fd, "I/O Data\t\t\t\t%f GiB\n", results.io_data);
}

void results_mmap_lat_eng(Arguments args, FILE *fd, Results results)
{
    fprintf(fd, "Iterations\t\t\t\t%ld\n", args.iterations);
    fprintf(fd, "Minimum latency\t\t\t\t%f usec\n", results.min_lat);
    fprintf(fd, "Maximum latency\t\t\t\t%f usec\n", results.max_lat);
    fprintf(fd, "Average latency\t\t\t\t%f usec\n", results.avg_lat);
}