#include "mmap.h"

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
    double elapsed = 0.0;

    mmap_check_args(args);

    for (uint64_t i = 0; i < args->iterations; i++)
    {
        mmap_prepare_mapping(&mapping, *args);
        elapsed += mmap_run_benchmark(&mapping, args, &results);
        mmap_cleanup_mapping(&mapping);
    }

    results.avg_lat = ((double)elapsed / (double)args->iterations / (double)args->cpy_iter) * SECS_TO_NANS;
    results.io_data = (double)args->iterations * (double)args->cpy_iter * (double)args->buflen;
    args->runtime = elapsed;
    args->size = mapping.size;
    mapping.size = 0;
    dump_results(results, *args);
}

void mmap_prepare_mapping(Mapping *mapping, Arguments args)
{
    if (args.map_anon)
        mmap_prepare_map_anon(mapping, &args);
    else
    {
        int fd;
        if ((fd = open(args.path, O_RDWR, 0666)) < 0)
            LOG(ERROR, errno, "File Error");

        mapping->size = lseek(fd, 0, SEEK_END);
        mapping->size -= mapping->size % PAGESIZE; // Align size to PAGE_SIZE

        int flags = set_flags(args);
        if ((mapping->addr = (char *)mmap(0, mapping->size, PROT_WRITE | PROT_READ, flags, fd, 0)) == MAP_FAILED)
        {
            close(fd);
            LOG(ERROR, errno, "mmap");
        }

        close(fd);

        mapping->map_anon = 0;
        mapping->fpath = args.path;
    }
}

// Mapping is anonymous
void mmap_prepare_map_anon(Mapping *mapping, Arguments *args)
{
    if (args->size < (uint64_t)PAGESIZE)
    {
        LOG(INFO, 0, "Too small size, aligning to minimum PAGE_SIZE");
        args->size = PAGESIZE;
    }
    args->size += args->size % PAGESIZE; // Align size to PAGE_SIZE

    int flags = set_flags(*args);

    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, args->size, PROT_WRITE | PROT_READ, flags, -1, 0)) == MAP_FAILED)
        LOG(ERROR, errno, "mmap");

    mapping->map_anon = 1;
    mapping->size = args->size;
    mmap_init_mem(mapping);
}

void mmap_init_mem(Mapping *mapping)
{
    srand(time(NULL));
    memset(mapping->addr, rand(), mapping->size);
}

void mmap_cleanup_mapping(Mapping *mapping)
{
    munmap(mapping->addr, mapping->size);

    mapping->addr = 0;
    mapping->buflen = 0;
    mapping->fpath = "";
    mapping->map_anon = 0;
}

double mmap_run_benchmark(Mapping *mapping, Arguments *args, Results *results)
{
    mapping->buflen = args->buflen;
    uint64_t counter = 0, elapsed = 0;

    // Dividing mapping into buflen sized chunks
    uint64_t max_ind = mapping->size / mapping->buflen;

    // If user specified iterations run those, otherwise use maximum iterations on mapping
    uint64_t loop_iters = (args->cpy_iter > 1 && max_ind > args->cpy_iter) ? args->cpy_iter : max_ind;
    double secs_elapsed = 0;
    unsigned char *buf = (unsigned char *)calloc(mapping->buflen * sizeof(char), 1);
    char **block_index = (char **)malloc(max_ind * sizeof(char *));

    // Storing pointers to beginning of every consecutive buflen sized chunk from file
    if (args->mode < 2)
    {
        for (uint64_t i = 0; i < max_ind; i++)
            block_index[i] = (char *)(mapping->addr + (mapping->buflen * i));
    }
    else // Storing pointers to buflen sized chunks in random order
    {
        srand(time(NULL));
        for (uint64_t i = 0; i < max_ind; i++)
            block_index[i] = (char *)(mapping->addr + (mapping->buflen * (rand() % max_ind)));
        madvise(mapping->addr, mapping->size, MADV_RANDOM);
    }

    if (args->mode == 1 || args->mode == 3)
    {
        // Fill buffer with random data for copying to memory
        for (size_t i = 0; i < mapping->buflen; i++)
            buf[i] = rand() % 256;
    }

    struct timespec tstart = {0, 0}, tend = {0, 0};

    while (secs_elapsed < args->runtime)
    {
        msync(mapping->addr, mapping->size, MS_INVALIDATE); // Invalidate cached data
        clock_gettime(CLOCK_MONOTONIC, &tstart);

        if (args->mode == 0 || args->mode == 2)
        {
            for (uint64_t i = 0; i < loop_iters; i++)
                memcpy(buf, block_index[i], mapping->buflen * sizeof(char));
        }
        else
        {
            for (uint64_t i = 0; i < loop_iters; i++)
                memcpy(block_index[i], buf, mapping->buflen * sizeof(char));
        }

        clock_gettime(CLOCK_MONOTONIC, &tend);
        counter += loop_iters;
        elapsed = NANS_ELAPSED(tend, tstart);
        add_latency(((double)elapsed / (double)loop_iters), results); // Adding the average latency for the run to be stored

        // Used to keep track of runtime
        secs_elapsed += elapsed * NANS_TO_SECS;

        if (args->cpy_iter > 1 && args->cpy_iter <= counter)
            break;
    }

    get_bandwidth(counter, secs_elapsed, mapping->buflen, results);
    args->runtime = secs_elapsed;
    args->cpy_iter = counter;

    free(buf);
    buf = NULL;
    free(block_index);
    block_index = NULL;

    return secs_elapsed;
}

// Check if all args are valid for engine to start
void mmap_check_args(Arguments *args)
{
    if (args->runtime < 1)
        LOG(ERROR, EINVAL, "Runtime should be greater than 1sec");
    if (args->size < args->buflen)
        LOG(ERROR, EINVAL, "Copysize can't be larger than size");
}