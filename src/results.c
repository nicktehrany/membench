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

    switch (args.engine)
    {
    case 0:
        fprintf(fd, "Engine\t\t\t\t\tmmap\n");
        break;
    case 1:
        fprintf(fd, "Engine\t\t\t\t\tmmap_lat\n");
        break;
    case 2:
        fprintf(fd, "Engine\t\t\t\t\tmem_lat\n");
        break;
    case 3:
        fprintf(fd, "Engine\t\t\t\t\tpage_fault\n");
        break;
    }

    if (args.engine == 0)
        results_mmap_eng(fd, results, args);
    else if (args.engine == 1)
        results_mmap_lat_eng(fd, results, args);
    else if (args.engine == 2)
        results_mem_lat_eng(fd, results, args);
    else if (args.engine == 3)
        results_page_fault_eng(fd, results, args);

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

void results_mmap_eng(FILE *fd, Results results, Arguments args)
{
    print_dir(fd, args);
    print_flags(fd, args);
    print_misc(fd, args);
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

    if (results.io_data >= (1024 * 1024 * 1024))
        fprintf(fd, "Data Copied\t\t\t\t%ld GiB\n", results.io_data / ((1024 * 1024 * 1024)));
    else if (results.io_data >= (1024 * 1024))
        fprintf(fd, "Data Copied\t\t\t\t%ld MiB\n", results.io_data / ((1024 * 1024)));
    else if (results.io_data >= 1024)
        fprintf(fd, "Data Copied\t\t\t\t%ld KiB\n", results.io_data / 1024);
    else
        fprintf(fd, "Data Copied\t\t\t\t%ld Bytes\n", results.io_data);

    fprintf(fd, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    fprintf(fd, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    fprintf(fd, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
}

void results_mmap_lat_eng(FILE *fd, Results results, Arguments args)
{
    print_dir(fd, args);
    print_flags(fd, args);
    print_misc(fd, args);
    fprintf(fd, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    fprintf(fd, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    fprintf(fd, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
}

void results_mem_lat_eng(FILE *fd, Results results, Arguments args)
{
    if (args.size >= (1024 * 1024 * 1024))
        fprintf(fd, "Buffer Size\t\t\t\t%ld GiB\n", args.size / (1024 * 1024 * 1024));
    else if (args.size >= (1024 * 1024))
        fprintf(fd, "Buffer Size\t\t\t\t%ld MiB\n", args.size / (1024 * 1024));
    else if (args.size >= 1024)
        fprintf(fd, "Buffer Size\t\t\t\t%ld KiB\n", args.size / 1024);
    else
        fprintf(fd, "File Size\t\t\t\t%ld Bytes\n", args.size);
    fprintf(fd, "Iterations\t\t\t\t%ld\n", args.iterations);
    fprintf(fd, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    fprintf(fd, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    fprintf(fd, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
}

void print_dir(FILE *fd, Arguments args)
{
    if (strcmp(args.path, "file") == 0)
        fprintf(fd, "Directory\t\t\t\tCurrent Directory\n");
    else if (args.engine != 2)
        fprintf(fd, "Directory\t\t\t\t%s\n", args.path);
}

void print_flags(FILE *fd, Arguments args)
{
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
}

void print_misc(FILE *fd, Arguments args)
{

    fprintf(fd, "Iterations\t\t\t\t%ld\n", args.iterations);
    fprintf(fd, "Memcpy Iterations\t\t%ld\n", args.cpy_iter);
    fprintf(fd, "Total Memcpy Calls\t\t%ld\n", args.iterations * args.cpy_iter);
    fprintf(fd, "Total Runtime\t\t\t%.6f sec\n", args.runtime);
    if (args.size >= (1024 * 1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld GiB\n", args.size / (1024 * 1024 * 1024));
    else if (args.size >= (1024 * 1024))
        fprintf(fd, "File Size\t\t\t\t%ld MiB\n", args.size / (1024 * 1024));
    else if (args.size >= 1024)
        fprintf(fd, "File Size\t\t\t\t%ld KiB\n", args.size / 1024);
    else
        fprintf(fd, "File Size\t\t\t\t%ld Bytes\n", args.size);
}

void results_page_fault_eng(FILE *fd, Results results, Arguments args)
{
    print_dir(fd, args);
    fprintf(fd, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    fprintf(fd, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    fprintf(fd, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
}
