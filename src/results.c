#include "results.h"

// Summarize all Arguments and Results in a file
void dump_results(Results results, Arguments args)
{
    FILE *fp = fopen("results.out", "w+");
    if (fp == NULL)
        LOG(ERROR, EINVAL, "Failed opening results.out file. Delete it or run as sudo!");
    fprintf(fp, "\t\t==== SUMMARY ====\n");

    switch (args.engine)
    {
    case 0:
        fprintf(fp, "Engine\t\t\t\t\tmmap\n");
        printf("Engine: mmap\n");
        break;
    case 1:
        fprintf(fp, "Engine\t\t\t\t\tmmap_lat\n");
        printf("Engine: mmap_lat\n");
        break;
    case 2:
        fprintf(fp, "Engine\t\t\t\t\tmem_lat\n");
        printf("Engine: mem_lat\n");
        break;
    case 3:
        fprintf(fp, "Engine\t\t\t\t\tpage_fault\n");
        printf("Engine: page_fault\n");
        break;
    }

    if (args.engine == 0)
        results_mmap_eng(fp, results, args);
    else if (args.engine == 1)
        results_mmap_lat_eng(fp, results, args);
    else if (args.engine == 2)
        results_mem_lat_eng(fp, results, args);
    else if (args.engine == 3)
        results_page_fault_eng(fp, results, args);

    fclose(fp);
}

void results_mmap_eng(FILE *fp, Results results, Arguments args)
{
    print_dir(fp, args);
    print_flags(fp, args);
    print_misc(fp, args);
    if (args.buflen >= (1024 * 1024 * 1024))
    {
        fprintf(fp, "Copy Size\t\t\t\t%ld GiB\n", args.buflen / ((1024 * 1024 * 1024)));
        printf("Copy Size: %ld GiB\n", args.buflen / ((1024 * 1024 * 1024)));
    }
    else if (args.buflen >= (1024 * 1024))
    {
        fprintf(fp, "Copy Size\t\t\t\t%ld MiB\n", args.buflen / ((1024 * 1024)));
        printf("Copy Size: %ld MiB\n", args.buflen / ((1024 * 1024)));
    }
    else if (args.buflen >= 1024)
    {
        fprintf(fp, "Copy Size\t\t\t\t%ld KiB\n", args.buflen / 1024);
        printf("Copy Size: %ld KiB\n", args.buflen / 1024);
    }
    else
    {
        fprintf(fp, "Copy Size\t\t\t\t%ld Bytes\n", args.buflen);
        printf("Copy Size: %ld Bytes\n", args.buflen);
    }

    switch (args.mode)
    {
    case 0:
        fprintf(fp, "Sequential Read\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        printf("Sequential Read: %f GiB/s\n", results.bandwidth / 1024);
        break;
    case 1:
        fprintf(fp, "Sequential Write\t\t%f GiB/s\n", results.bandwidth / 1024);
        printf("Sequential Write: %f GiB/s\n", results.bandwidth / 1024);
        break;
    case 2:
        fprintf(fp, "Random Read\t\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        printf("Random Read: %f GiB/s\n", results.bandwidth / 1024);
        break;
    case 3:
        fprintf(fp, "Random Write\t\t\t%f GiB/s\n", results.bandwidth / 1024);
        printf("Random Write: %f GiB/s\n", results.bandwidth / 1024);
        break;
    }

    if (results.io_data >= (1024 * 1024 * 1024))
    {
        fprintf(fp, "Data Copied\t\t\t\t%ld GiB\n", results.io_data / ((1024 * 1024 * 1024)));
        printf("Data Copied: %ld GiB\n", results.io_data / ((1024 * 1024 * 1024)));
    }
    else if (results.io_data >= (1024 * 1024))
    {
        fprintf(fp, "Data Copied\t\t\t\t%ld MiB\n", results.io_data / ((1024 * 1024)));
        printf("Data Copied: %ld MiB\n", results.io_data / ((1024 * 1024)));
    }
    else if (results.io_data >= 1024)
    {
        fprintf(fp, "Data Copied\t\t\t\t%ld KiB\n", results.io_data / 1024);
        printf("Data Copied: %ld KiB\n", results.io_data / 1024);
    }
    else
    {
        fprintf(fp, "Data Copied\t\t\t\t%ld Bytes\n", results.io_data);
        printf("Data Copied: %ld Bytes\n", results.io_data);
    }

    fprintf(fp, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    printf("Minimum latency: %.2f nsec\n", results.min_lat);
    fprintf(fp, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    printf("Maximum latency: %.2f nsec\n", results.max_lat);
    fprintf(fp, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
    printf("Average latency: %.2f nsec\n", results.avg_lat);
}

void results_mmap_lat_eng(FILE *fp, Results results, Arguments args)
{
    print_dir(fp, args);
    print_flags(fp, args);
    print_misc(fp, args);
    fprintf(fp, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    printf("Minimum latency: %.2f nsec\n", results.min_lat);
    fprintf(fp, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    printf("Maximum latency: %.2f nsec\n", results.max_lat);
    fprintf(fp, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
    printf("Average latency: %.2f nsec\n", results.avg_lat);
}

void results_mem_lat_eng(FILE *fp, Results results, Arguments args)
{
    if (args.size >= (1024 * 1024 * 1024))
    {
        fprintf(fp, "Buffer Size\t\t\t\t%ld GiB\n", args.size / (1024 * 1024 * 1024));
        printf("Buffer Size: %ld GiB\n", args.size / (1024 * 1024 * 1024));
    }
    else if (args.size >= (1024 * 1024))
    {
        fprintf(fp, "Buffer Size\t\t\t\t%ld MiB\n", args.size / (1024 * 1024));
        printf("Buffer Size: %ld MiB\n", args.size / (1024 * 1024));
    }
    else if (args.size >= 1024)
    {
        fprintf(fp, "Buffer Size\t\t\t\t%ld KiB\n", args.size / 1024);
        printf("Buffer Size: %ld KiB\n", args.size / 1024);
    }
    else
    {
        fprintf(fp, "File Size\t\t\t\t%ld Bytes\n", args.size);
        printf("File Size: %ld Bytes\n", args.size);
    }
    fprintf(fp, "Iterations\t\t\t\t%ld\n", args.iterations);
    printf("Iterations: %ld\n", args.iterations);
    fprintf(fp, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    printf("Minimum latency: %.2f nsec\n", results.min_lat);
    fprintf(fp, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    printf("Maximum latency: %.2f nsec\n", results.max_lat);
    fprintf(fp, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
    printf("Average latency: %.2f nsec\n", results.avg_lat);
}

void print_dir(FILE *fp, Arguments args)
{
    if (strcmp(args.path, "file") == 0)
        fprintf(fp, "Directory\t\t\t\tCurrent Directory\n");
    else if (args.engine != 2)
        fprintf(fp, "Directory\t\t\t\t%s\n", args.path);
}

void print_flags(FILE *fp, Arguments args)
{
    fprintf(fp, "Flags\t\t\t\t\t");
    printf("Flags: ");
    switch (args.map_anon)
    {
    case 1:
        fprintf(fp, "MAP_ANONYMOUS ");
        printf("MAP_ANONYMOUS ");
        break;
    default:
        break;
    }
    switch (args.map_pop)
    {
    case 1:
        fprintf(fp, "MAP_POPULATE ");
        printf("MAP_POPULATE ");
        break;
    default:
        break;
    }
    switch (args.map_shared)
    {
    case 1:
        fprintf(fp, "MAP_SHARED ");
        printf("MAP_SHARED ");
        break;
    default:
        fprintf(fp, "MAP_PRIVATE ");
        printf("MAP_PRIVATE ");
        break;
    }
    fprintf(fp, "\n");
    printf("\n");
}

void print_misc(FILE *fp, Arguments args)
{

    fprintf(fp, "Iterations\t\t\t\t%ld\n", args.iterations);
    printf("Iterations: %ld\n", args.iterations);
    fprintf(fp, "Memcpy Iterations\t\t%ld\n", args.cpy_iter);
    printf("Memcpy Iterations: %ld\n", args.cpy_iter);
    fprintf(fp, "Total Memcpy Calls\t\t%ld\n", args.iterations * args.cpy_iter);
    printf("Total Memcpy Calls: %ld\n", args.iterations * args.cpy_iter);
    fprintf(fp, "Total Runtime\t\t\t%.6f sec\n", args.runtime);
    printf("Total Runtime: %.6f sec\n", args.runtime);
    if (args.size >= (1024 * 1024 * 1024))
    {
        fprintf(fp, "File Size\t\t\t\t%ld GiB\n", args.size / (1024 * 1024 * 1024));
        printf("File Size: %ld GiB\n", args.size / (1024 * 1024 * 1024));
    }
    else if (args.size >= (1024 * 1024))
    {
        fprintf(fp, "File Size\t\t\t\t%ld MiB\n", args.size / (1024 * 1024));
        printf("File Size: %ld MiB\n", args.size / (1024 * 1024));
    }
    else if (args.size >= 1024)
    {
        fprintf(fp, "File Size\t\t\t\t%ld KiB\n", args.size / 1024);
        printf("File Size: %ld KiB\n", args.size / 1024);
    }
    else
    {
        fprintf(fp, "File Size\t\t\t\t%ld Bytes\n", args.size);
        printf("File Size: %ld Bytes\n", args.size);
    }
}

void results_page_fault_eng(FILE *fp, Results results, Arguments args)
{
    print_dir(fp, args);
    fprintf(fp, "Minimum latency\t\t\t%.2f nsec\n", results.min_lat);
    printf("Minimum latency: %.2f nsec\n", results.min_lat);
    fprintf(fp, "Maximum latency\t\t\t%.2f nsec\n", results.max_lat);
    printf("Maximum latency: %.2f nsec\n", results.max_lat);
    fprintf(fp, "Average latency\t\t\t%.2f nsec\n", results.avg_lat);
    printf("Average latency: %.2f nsec\n", results.avg_lat);
}
