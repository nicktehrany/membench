#include "mmap_lat.h"

/*
 *
 * Engine that will continuously, for the given number of iterations mmap
 * and unmap a file, of given size, and measure the latency of the mmap
 * call. Possible options are regular file on any mounted file system and
 * MAP_ANONYMOUS, for no file backing. Each one can be run with or without
 * MAP_POPULATE, to pre populate pages
 */
void mmap_lat_engine(Arguments *args)
{
    Results results = {0, 0, 0, 0, 0};
    Mapping mapping = {0, 0, 0, 0, ""};
    uint64_t acc_nsecs = 0;
    int fd = 0;
    uint64_t latency = 0;

    mmap_lat_check_args(args);
    fd = mmap_lat_prep_file(args);
    mapping.fpath = args->path;
    mapping.size = args->size;
    mapping.map_anon = args->map_anon;

    for (uint64_t i = 0; i < args->iterations; i++)
    {
        latency = mmap_lat_do_mmap(&mapping, *args, fd);
        add_latency(latency, &results);
        acc_nsecs += latency;
        mmap_lat_do_unmap(&mapping);
    }

    args->runtime = acc_nsecs * NANS_TO_SECS;
    results.avg_lat = (double)acc_nsecs / (double)args->iterations;
    dump_results(results, *args);
}

int mmap_lat_prep_file(Arguments *args)
{
    int fd;

    if (args->map_anon)
        fd = -1;
    else
    {
        if ((fd = open(args->path, O_RDWR, 0666)) < 0)
            LOG(ERROR, errno, "File Open");

        args->size = lseek(fd, 0, SEEK_END);
        args->size -= args->size % PAGESIZE; // Align size to PAGE_SIZE
    }
    return fd;
}

uint64_t mmap_lat_do_mmap(Mapping *mapping, Arguments args, int fd)
{
    uint64_t nsecs_elapsed = 0;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    if (args.map_anon)
        nsecs_elapsed = mmap_lat_do_mmap_anon(mapping, args, fd);
    else
    {
        int flags = set_flags(args);

        clock_gettime(CLOCK_MONOTONIC, &tstart);
        if ((mapping->addr = (char *)mmap(0, args.size, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
        {
            close(fd);
            LOG(ERROR, errno, "mmap");
        }
        clock_gettime(CLOCK_MONOTONIC, &tend);
        nsecs_elapsed = NANS_ELAPSED(tend, tstart);
    }
    return nsecs_elapsed;
}

// Mapping is anonymous
uint64_t mmap_lat_do_mmap_anon(Mapping *mapping, Arguments args, int fd)
{
    uint64_t nsecs_elapsed = 0;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    int flags = set_flags(args);

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, args.size, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
        LOG(ERROR, errno, "mmap");

    clock_gettime(CLOCK_MONOTONIC, &tend);
    nsecs_elapsed = NANS_ELAPSED(tend, tstart);

    return nsecs_elapsed;
}

void mmap_lat_do_unmap(Mapping *mapping)
{
    munmap(mapping->addr, mapping->size);
}

// Check if all args are valid for engine to start
void mmap_lat_check_args(Arguments *args)
{
    if (strcmp(args->path, "") == 0)
        LOG(ERROR, EINVAL, "Missing File Path");
}