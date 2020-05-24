#include "mmap.h"
#define PAGESIZE sysconf(_SC_PAGE_SIZE)

/*
 *
 * Engine that uses mmap to map files into memory and then measures throughput by 
 * reading/writing from and to it using memcpy. Possible file options are a usual
 * file from any mounted file system, MAP_ANONYMOUS for no file backing.
 * Engine will create and delete a file of the specified sizes (which can take longer 
 * for large files).
*/
void mmap_engine(Arguments *args)
{
    Results results = {0, 0, 0, 0, 0};
    Mapping mapping = {0, 0, 0, 0, ""};

    mmap_check_args(args);
    mmap_prepare_mapping(&mapping, *args);
    mmap_run_benchmark(&mapping, args, &results);
    mmap_cleanup_mapping(&mapping);
    dump_results(results, *args);
}

void mmap_seq_read(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, elapsed = 0;

    // If buflen > PAGE_SIZE, divide file into buflen size chunks else PAGE_SIZE chunks
    uint64_t chunk_size = (mapping->buflen > (uint64_t)PAGESIZE) ? mapping->buflen : (uint64_t)PAGESIZE;
    uint64_t max_ind = mapping->fsize / chunk_size;
    uint64_t loop_iters = (max_ind > args->iterations && args->iterations != 0) ? args->iterations : max_ind;
    double secs_elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    // Storing pointers to beginning of every consecutive buflen/PAGE_SIZE sized chunk from file
    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (i * chunk_size));

    struct timespec tstart = {0, 0}, tend = {0, 0};

    while (secs_elapsed < args->runtime)
    {
        clock_gettime(CLOCK_MONOTONIC, &tstart);

        // Reading from every consecutive chunk_size part of file at least once
        for (uint64_t i = 0; i < loop_iters; i++)
            memcpy(dest, block_index[i], mapping->buflen * sizeof(char));

        clock_gettime(CLOCK_MONOTONIC, &tend);
        counter += loop_iters;
        elapsed = NANS_ELAPSED(tend, tstart);
        add_latency(((double)elapsed / (double)loop_iters), results); // Adding the average latency for the run to be stored

        // Used to keep track of runtime
        secs_elapsed += elapsed * NANS_TO_SECS;

        if (args->iterations != 0 && args->iterations <= counter)
            break;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->iterations = counter;
    args->runtime = secs_elapsed;

    free(dest);
    dest = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_rand_read(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, elapsed = 0;
    uint64_t chunk_size = (mapping->buflen > (uint64_t)PAGESIZE) ? mapping->buflen : (uint64_t)PAGESIZE;
    uint64_t max_ind = mapping->fsize / chunk_size;
    uint64_t loop_iters = (max_ind > args->iterations && args->iterations != 0) ? args->iterations : max_ind;
    double secs_elapsed = 0;
    unsigned char *dest = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    // Storing random pointers from file in random order
    srand(time(NULL));
    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (rand() % mapping->fsize));

    struct timespec tstart = {0, 0}, tend = {0, 0};

    madvise(mapping->addr, mapping->fsize, MADV_RANDOM);

    while (secs_elapsed < args->runtime)
    {
        clock_gettime(CLOCK_MONOTONIC, &tstart);

        for (uint64_t i = 0; i < loop_iters; i++)
            memcpy(dest, block_index[i], mapping->buflen * sizeof(char));

        clock_gettime(CLOCK_MONOTONIC, &tend);
        counter += loop_iters;
        elapsed = NANS_ELAPSED(tend, tstart);
        add_latency(((double)elapsed / (double)loop_iters), results);
        secs_elapsed += elapsed * NANS_TO_SECS;

        if (args->iterations != 0 && args->iterations <= counter)
            break;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->iterations = counter;
    args->runtime = secs_elapsed;

    free(dest);
    dest = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_seq_write(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, elapsed = 0;
    uint64_t chunk_size = (mapping->buflen > (uint64_t)PAGESIZE) ? mapping->buflen : (uint64_t)PAGESIZE;
    uint64_t max_ind = mapping->fsize / chunk_size;
    uint64_t loop_iters = (max_ind > args->iterations && args->iterations != 0) ? args->iterations : max_ind;
    double secs_elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (i * chunk_size));

    // Filling src with rand bytes to memcpy to dest
    srand(time(NULL));
    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    struct timespec tstart = {0, 0}, tend = {0, 0};

    while (secs_elapsed < args->runtime)
    {
        clock_gettime(CLOCK_MONOTONIC, &tstart);

        for (uint64_t i = 0; i < loop_iters; i++)
            memcpy(block_index[i], src, mapping->buflen * sizeof(char));

        clock_gettime(CLOCK_MONOTONIC, &tend);
        counter += loop_iters;
        elapsed = NANS_ELAPSED(tend, tstart);
        add_latency(((double)elapsed / (double)loop_iters), results);
        secs_elapsed += elapsed * NANS_TO_SECS;

        if (args->iterations != 0 && args->iterations <= counter)
            break;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->iterations = counter;
    args->runtime = secs_elapsed;

    free(src);
    src = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_rand_write(Mapping *mapping, Results *results, Arguments *args)
{
    uint64_t counter = 0, elapsed = 0;
    uint64_t chunk_size = (mapping->buflen > (uint64_t)PAGESIZE) ? mapping->buflen : (uint64_t)PAGESIZE;
    uint64_t max_ind = mapping->fsize / chunk_size;
    uint64_t loop_iters = (max_ind > args->iterations && args->iterations != 0) ? args->iterations : max_ind;
    double secs_elapsed = 0;
    unsigned char *src = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    for (uint64_t i = 0; i < max_ind; i++)
        block_index[i] = (char *)(mapping->addr + (rand() % mapping->fsize));

    srand(time(NULL));
    for (uint64_t i = 0; i < mapping->buflen; i++)
        src[i] = rand() % 256;

    struct timespec tstart = {0, 0}, tend = {0, 0};

    madvise(mapping->addr, mapping->fsize, MADV_RANDOM);

    while (secs_elapsed < args->runtime)
    {
        clock_gettime(CLOCK_MONOTONIC, &tstart);

        for (uint64_t i = 0; i < loop_iters; i++)
            memcpy(block_index[i], src, mapping->buflen * sizeof(char));

        clock_gettime(CLOCK_MONOTONIC, &tend);
        counter += loop_iters;
        elapsed = NANS_ELAPSED(tend, tstart);
        add_latency(((double)elapsed / (double)loop_iters), results);
        secs_elapsed += elapsed * NANS_TO_SECS;

        if (args->iterations != 0 && args->iterations <= counter)
            break;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    results->avg_lat = (secs_elapsed * SECS_TO_NANS) / counter;
    args->iterations = counter;
    args->runtime = secs_elapsed;

    free(src);
    src = NULL;
    free(block_index);
    block_index = NULL;
}

void mmap_prepare_mapping(Mapping *mapping, Arguments args)
{
    if (args.map_anon)
        mmap_prepare_map_anon(mapping, args);
    else
    {
        int fd;

        // Open/Create file and truncate it to given size
        if (((fd = open(args.path, O_CREAT | O_RDWR, 0666)) < 0) || (ftruncate(fd, args.fsize) != 0))
        {
            perror("File Error");
            exit(1);
        }

        int flags = set_flags(args);
        if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            remove(args.path);
            exit(1);
        }

        close(fd);

        mapping->map_anon = 0;
        mapping->fsize = args.fsize;
        mapping->fpath = args.path;
    }
}

// Mapping is anonymous
void mmap_prepare_map_anon(Mapping *mapping, Arguments args)
{
    int flags = set_flags(args);

    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, flags, -1, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    mapping->map_anon = 1;
    mapping->fsize = args.fsize;
}

void mmap_cleanup_mapping(Mapping *mapping)
{
    munmap(mapping->addr, mapping->fsize);

    if (!mapping->map_anon && remove(mapping->fpath) != 0)
    {
        errno = EINVAL;
        perror("Error deleting file");
    }

    mapping->addr = 0;
    mapping->buflen = 0;
    mapping->fpath = "";
    mapping->fsize = 0;
    mapping->map_anon = 0;
}

void mmap_run_benchmark(Mapping *mapping, Arguments *args, Results *results)
{
    mapping->buflen = args->buflen;
    switch (args->mode)
    {
    case 0:
        mmap_seq_read(mapping, results, args);
        break;
    case 1:
        mmap_seq_write(mapping, results, args);
        break;
    case 2:
        mmap_rand_read(mapping, results, args);
        break;
    case 3:
        mmap_rand_write(mapping, results, args);
        break;
    default:
        errno = EINVAL;
        perror("Invalid Mode");
        exit(1);
    }
}

// Check if all args are valid for engine to start
void mmap_check_args(Arguments *args)
{
    if (strcmp(args->path, "") == 0)
    {
        args->path = "file";
    }
    if (args->buflen <= 0 || args->fsize <= 0)
    {
        errno = EINVAL;
        perror("Invalid or missing file or copy size");
        exit(1);
    }
    if (args->runtime < 1)
    {
        errno = EINVAL;
        perror("Runtime should be greater than 1sec");
        exit(1);
    }
    if (args->buflen > args->fsize)
    {
        errno = EINVAL;
        perror("Copy size can't be larger than file size");
        exit(1);
    }
    if (args->fsize % args->buflen != 0)
    {
        errno = EINVAL;
        perror("Not aligned file size and copy size");
        exit(1);
    }
}