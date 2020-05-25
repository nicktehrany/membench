#include "page_fault.h"
#define ITERATIONS 100
#define FACTOR 25

void page_fault_lat_engine(Arguments *args)
{
    PageMap pagemap;
    Results results = {0, 0, 0, 0, 0};
    double latency = 0.0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        page_fault_setup(&pagemap, args);
        latency += page_fault_benchmark(&pagemap, &results);
        page_fault_unmap(&pagemap);
    }
    results.avg_lat = latency / ITERATIONS;
    dump_results(results, *args);
}

void page_fault_setup(PageMap *pagemap, Arguments *args)
{
    int fd;
    if ((fd = open(args->path, O_RDWR, 0666)) < 0)
    {
        perror("File Error");
        exit(1);
    }

    pagemap->size = lseek(fd, 0, SEEK_END);
    pagemap->size -= pagemap->size % PAGESIZE; // Align size to PAGE_SIZE

    if ((pagemap->base_ptr = (char *)mmap(0, pagemap->size, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(1);
    }

    close(fd);
    madvise(pagemap->base_ptr, pagemap->size, MADV_RANDOM);
    msync(pagemap->base_ptr, pagemap->size, MS_INVALIDATE); // Invalidate mapping in case it's cached
}

double page_fault_benchmark(PageMap *pagemap, Results *results)
{
    uint64_t num_pages = pagemap->size / PAGESIZE;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t *page_index = (size_t *)malloc(num_pages * sizeof(size_t));
    uint64_t elapsed = 0;

    for (uint64_t i = 0; i < num_pages; i++)
        page_index[i] = i;

    shuffle_page_index(page_index, num_pages);
    num_pages = num_pages / FACTOR; // Only using a percentage of mapping to avoid any kind of prefetching or caching

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (uint64_t i = 0; i < num_pages; i++)
        pagemap->sum += *(pagemap->base_ptr + page_index[i] * PAGESIZE);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    elapsed = NANS_ELAPSED(tend, tstart);
    add_latency(elapsed / num_pages, results);

    free(page_index);
    page_index = NULL;

    return elapsed / num_pages;
}

void page_fault_unmap(PageMap *pagemap)
{
    munmap(pagemap->base_ptr, pagemap->size);
    pagemap->base_ptr = NULL;
    pagemap->size = 0;
}

// Shuffling the page indices around for random access
void shuffle_page_index(uint64_t *page_index, size_t size)
{
    srand(time(NULL));
    for (uint64_t i = 0; i < size - 1; i++)
    {
        uint64_t j = rand() % size;
        int t = page_index[j];
        page_index[j] = page_index[i];
        page_index[i] = t;
    }
}