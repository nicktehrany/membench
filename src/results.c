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
    Size_Unit size_unit;

    format_size(&size_unit, args.buflen);
    fprintf(fp, "Copy Size\t\t\t\t%.0Lf %s\n", size_unit.size, size_unit.unit);
    printf("Copy Size: %.0Lf %s\n", size_unit.size, size_unit.unit);

    format_size(&size_unit, results.bandwidth);
    switch (args.mode)
    {
    case 0:
        fprintf(fp, "Sequential Read\t\t\t%Lf %s/s\n", size_unit.size, size_unit.unit);
        printf("Sequential Read: %Lf %s/s\n", size_unit.size, size_unit.unit);
        break;
    case 1:
        fprintf(fp, "Sequential Write\t\t%Lf %s/s\n", size_unit.size, size_unit.unit);
        printf("Sequential Write: %Lf %s/s\n", size_unit.size, size_unit.unit);
        break;
    case 2:
        fprintf(fp, "Random Read\t\t\t\t%Lf %s/s\n", size_unit.size, size_unit.unit);
        printf("Random Read: %Lf %s/s\n", size_unit.size, size_unit.unit);
        break;
    case 3:
        fprintf(fp, "Random Write\t\t\t%Lf %s/s\n", size_unit.size, size_unit.unit);
        printf("Random Write: %Lf %s/s\n", size_unit.size, size_unit.unit);
        break;
    }

    format_size(&size_unit, results.io_data);
    fprintf(fp, "Data Copied\t\t\t\t%Lf %s\n", size_unit.size, size_unit.unit);
    printf("Data Copied: %Lf %s\n", size_unit.size, size_unit.unit);

    print_latencies(fp, results);
}

void results_mmap_lat_eng(FILE *fp, Results results, Arguments args)
{
    print_dir(fp, args);
    print_flags(fp, args);
    printf("Iterations: %ld\n", args.iterations);
    fprintf(fp, "Memcpy Calls\t\t%ld\n", args.iterations * args.iterations);
    print_latencies(fp, results);
}

void results_mem_lat_eng(FILE *fp, Results results, Arguments args)
{
    Size_Unit size_unit;

    format_size(&size_unit, args.size);

    fprintf(fp, "Buffer Size\t\t\t\t%.0Lf %s\n", size_unit.size, size_unit.unit);
    printf("Buffer Size: %.0Lf %s\n", size_unit.size, size_unit.unit);

    fprintf(fp, "Iterations\t\t\t\t%ld\n", args.iterations);
    printf("Iterations: %ld\n", args.iterations);
    print_latencies(fp, results);
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
    Size_Unit size_unit;

    fprintf(fp, "Iterations\t\t\t\t%ld\n", args.iterations);
    printf("Iterations: %ld\n", args.iterations);
    fprintf(fp, "Memcpy Iterations\t\t%ld\n", args.cpy_iter);
    printf("Memcpy Iterations: %ld\n", args.cpy_iter);
    fprintf(fp, "Total Memcpy Calls\t\t%ld\n", args.iterations * args.cpy_iter);
    printf("Total Memcpy Calls: %ld\n", args.iterations * args.cpy_iter);
    fprintf(fp, "Total Runtime\t\t\t%.6f sec\n", args.runtime);
    printf("Total Runtime: %.6f sec\n", args.runtime);

    format_size(&size_unit, args.size);

    fprintf(fp, "File Size\t\t\t\t%.0Lf %s\n", size_unit.size, size_unit.unit);
    printf("File Size: %.0Lf %s\n", size_unit.size, size_unit.unit);
}

void print_latencies(FILE *fp, Results results)
{
    Size_Unit size_unit;

    format_latency(&size_unit, results.min_lat);
    fprintf(fp, "Minimum latency\t\t\t%.2LF %s\n", size_unit.size, size_unit.unit);
    printf("Minimum latency: %.2LF %s\n", size_unit.size, size_unit.unit);
    format_latency(&size_unit, results.max_lat);
    fprintf(fp, "Maximum latency\t\t\t%.2LF %s\n", size_unit.size, size_unit.unit);
    printf("Maximum latency: %.2LF %s\n", size_unit.size, size_unit.unit);
    format_latency(&size_unit, results.avg_lat);
    fprintf(fp, "Average latency\t\t\t%.2LF %s\n", size_unit.size, size_unit.unit);
    printf("Average latency: %.2LF %s\n", size_unit.size, size_unit.unit);
}

void results_page_fault_eng(FILE *fp, Results results, Arguments args)
{
    print_dir(fp, args);
    print_latencies(fp, results);
}

void format_size(Size_Unit *size_unit, uint64_t size)
{
    if (size >= (1024 * 1024 * 1024))
    {
        size_unit->size = size / ((1024.0 * 1024.0 * 1024.0));
        size_unit->unit = "GiB";
    }
    else if (size >= (1024 * 1024))
    {
        size_unit->size = size / ((1024.0 * 1024.0));
        size_unit->unit = "MiB";
    }
    else if (size >= 1024)
    {
        size_unit->size = size / 1024.0;
        size_unit->unit = "KiB";
    }
    else
    {
        size_unit->size = size;
        size_unit->unit = "B";
    }
}

void format_latency(Size_Unit *size_unit, uint64_t latency)
{
    if (latency >= (1000 * 1000))
    {
        size_unit->size = latency / ((1000.0 * 1000.0));
        size_unit->unit = "msec";
    }
    else if (latency >= 1000)
    {
        size_unit->size = latency / 1000.0;
        size_unit->unit = "usec";
    }
    else
    {
        size_unit->size = latency;
        size_unit->unit = "nsec";
    }
}
