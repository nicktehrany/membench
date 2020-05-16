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

    fprintf(fd, "Flags\t\t\t\t\t");
    switch (args.map_anon)
    {
    case 1:
        fprintf(fd, "MAP_ANONYMOUS ");
        break;
    default:
        break;
    }
    switch (args.map_pop)
    {
    case 1:
        fprintf(fd, "MAP_POPULATE ");
        break;
    default:
        break;
    }
    switch (args.map_shared)
    {
    case 1:
        fprintf(fd, "MAP_SHARED ");
        break;
    default:
        fprintf(fd, "MAP_PRIVATE ");
        break;
    }
    fprintf(fd, "\n");
    fprintf(fd, "Iterations\t\t\t\t%ld\n", args.iterations);
    fprintf(fd, "Total Runtime\t\t\t%ld sec\n", args.runtime);

    if (args.engine == 0)
        results_mmap_eng(args, fd, results);
    else if (args.engine == 1)
        results_mmap_lat_eng(fd, results);
    // display_results(fd); TODO IMPLEMENT FULLY
    fclose(fd);
}

void display_results(FILE *fp)
{
    char buff[255];
    while (fgets(buff, 255, (FILE *)fp))
    {
        printf("%s", buff);
    }
}

void results_mmap_eng(Arguments args, FILE *fd, Results results)
{
    fprintf(fd, "Memcpy Runtime\t\t\t%f sec\n", results.cpytime);
    if (args.fsize >= (1024 * 1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld GiB\n", args.fsize / (1024 * 1024 * 1024));
    else if (args.fsize >= (1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld MiB\n", args.fsize / (1024 * 1024));
    else if (args.fsize >= 1024)
        fprintf(fd, "File Size\t\t\t\t%ld KiB\n", args.fsize / 1024);
    else
        fprintf(fd, "File Size\t\t\t\t%ld Bytes\n", args.fsize);

    if (args.buflen >= (1024 * 1024 * 1024))
        fprintf(fd, "Copy Size\t\t\t\t%ld GiB\n", args.buflen / ((1024 * 1024 * 1024)));
    else if (args.buflen >= (1024 * 1024))
        fprintf(fd, "Copy Size\t\t\t\t%ld MiB\n", args.buflen / ((1024 * 1024)));
    else if (args.buflen >= 1024)
        fprintf(fd, "Copy Size\t\t\t\t%ld KiB\n", args.buflen / 1024);
    else
        fprintf(fd, "Copy Size\t\t\t\t%ld Bytes\n", args.buflen);

    switch (args.mode)
    {
    case 0:
        fprintf(fd, "Sequential Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 1:
        fprintf(fd, "Sequential Write\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 2:
        fprintf(fd, "Random Read\t\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    case 3:
        fprintf(fd, "Random Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        break;
    }

    fprintf(fd, "I/O Data\t\t\t\t%f GiB\n", results.io_data);
    //if (results.min_lat < 1000.0)
    fprintf(fd, "Minimum latency\t\t\t%.0f nsec\n", results.min_lat);
    // else
    //     fprintf(fd, "Minimum latency\t\t\t%f usec\n", results.min_lat / 1000.0);
    // if (results.max_lat < 1000.0)
    fprintf(fd, "Maximum latency\t\t\t%.0f nsec\n", results.max_lat);
    // else
    //     fprintf(fd, "Maximum latency\t\t\t%f usec\n", results.max_lat / 1000.0);
    // if (results.avg_lat < 1000.0)
    fprintf(fd, "Average latency\t\t\t%.0f nsec\n", results.avg_lat);
    // else
    //     fprintf(fd, "Average latency\t\t\t%f usec\n", results.avg_lat / 1000.0);
}

void results_mmap_lat_eng(FILE *fd, Results results)
{
    if (results.min_lat < 1000.0)
        fprintf(fd, "Minimum latency\t\t\t%.0f nsec\n", results.min_lat);
    else
        fprintf(fd, "Minimum latency\t\t\t%f usec\n", results.min_lat / 1000.0);
    if (results.max_lat < 1000.0)
        fprintf(fd, "Maximum latency\t\t\t%.0f nsec\n", results.max_lat);
    else
        fprintf(fd, "Maximum latency\t\t\t%f usec\n", results.max_lat / 1000.0);
    if (results.avg_lat < 1000.0)
        fprintf(fd, "Average latency\t\t\t%.0f nsec\n", results.avg_lat);
    else
        fprintf(fd, "Average latency\t\t\t%f usec\n", results.avg_lat / 1000.0);
}