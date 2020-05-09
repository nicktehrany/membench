#include "mmap_lat.h"
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

/*
 *
 * Engine that will continuously, for the given number of iterations mmap
 * and unmap a file, of given size, and measure the latency of the mmap
 * call. Possible options are regular file on any mounted file system and
 * MAP_ANONYMOUS, for no file backing. Each one can be run with or without
 * MAP_POPULATE, to pre populate pages
 */
void mmap_lat_engine(Mapping *mapping, Arguments *args, Results *results)
{
    uint64_t acc_usec = 0;
    int fd = 0;
    uint64_t value = 0;

    mmap_lat_check_args(args);
    fd = mmap_lat_prep_file(*args);
    mapping->fpath = args->path;
    mapping->fsize = args->fsize;
    mapping->map_anon = args->map_anon;

    for (uint64_t i = 0; i < args->iterations; i++)
    {
        value = mmap_lat_do_mmap(mapping, *args, fd);
        if (value > results->max_lat)
            results->max_lat = value;
        if (value < results->min_lat || results->min_lat == 0)
            results->min_lat = value;
        acc_usec += value;
        mmap_lat_do_unmap(mapping);
    }

    mmap_lat_cleanup_file(mapping, fd);
    results->avg_lat = (double)acc_usec / (double)args->iterations;
    dump_results(*results, *args);
}

int mmap_lat_prep_file(Arguments args)
{
    int fd;

    if (args.map_anon)
        fd = -1;
    else
    {
        if ((fd = open(args.path, O_CREAT | O_RDWR, 0666)) < 0)
        {
            perror("File Open");
            exit(1);
        }

        if (ftruncate(fd, args.fsize) != 0)
        {
            perror("truncate");
            close(fd);
            remove(args.path);
            exit(1);
        }
    }
    return fd;
}

uint64_t mmap_lat_do_mmap(Mapping *mapping, Arguments args, int fd)
{
    double elapsed = 0.0;
    if (args.map_anon)
        elapsed = mmap_lat_do_mmap_anon(mapping, args, fd);
    else
    {
        int flags;
        if (args.map_pop)
            flags = MAP_SHARED | MAP_POPULATE;
        else
            flags = MAP_SHARED;
        clock_t start = clock();
        if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            remove(args.path);
            exit(1);
        }
        clock_t end = clock();
        elapsed = (double)end - (double)start;
    }
    return elapsed;
}

// Mapping is anonymous
uint64_t mmap_lat_do_mmap_anon(Mapping *mapping, Arguments args, int fd)
{
    int flags;
    if (args.map_pop)
        flags = MAP_ANONYMOUS | MAP_SHARED | MAP_POPULATE;
    else
        flags = MAP_ANONYMOUS | MAP_SHARED;
    clock_t start = clock();
    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    clock_t end = clock();

    return ((double)end - (double)start);
}

void mmap_lat_do_unmap(Mapping *mapping)
{
    munmap(mapping->addr, mapping->fsize);
}

void mmap_lat_cleanup_file(Mapping *mapping, int fd)
{
    close(fd);
    if (!mapping->map_anon && remove(mapping->fpath) != 0)
    {
        errno = EINVAL;
        perror("Error deleting file");
    }
}

// Check if all args are valid for engine to start
void mmap_lat_check_args(Arguments *args)
{
    if (args->fsize <= 0)
    {
        errno = EINVAL;
        perror("Invalid or missing file or copy size");
        exit(1);
    }
    if (strcmp(args->path, "") == 0)
    {
        args->path = "file";
    }
}