#include "page_fault.h"
#define ITERATIONS 1
#define FACTOR 25

void page_fault_lat_engine(Arguments *args)
{
    PageMap pagemap;
    Results results = {0, 0, 0, 0, 0};

    for (int i = 0; i < ITERATIONS; i++)
    {
        page_fault_setup(&pagemap, args);
        page_fault_benchmark(&pagemap, &results);
        page_fault_unmap(&pagemap);
    }
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
    //msync(pagemap->base_ptr, pagemap->size, MS_INVALIDATE); // Drop it in case it's cached
}

void page_fault_benchmark(PageMap *pagemap, Results *results)
{
    uint64_t num_pages = pagemap->size / PAGESIZE;
    struct timespec tstart = {0, 0}, tend = {0, 0};
    uint64_t *page_index = (size_t *)malloc(num_pages * sizeof(size_t));

    for (uint64_t i = 0; i < num_pages; i++)
        page_index[i] = i;

    shuffle(page_index, num_pages);
    num_pages = 10; // Only using 4 percent of mapping to avoid any kind of prefetching or caching

    clock_t dummy = clock(); // DUMMY
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (uint64_t i = 0; i < num_pages; i++)
        pagemap->sum += *(pagemap->base_ptr + page_index[i] * PAGESIZE);

    clock_gettime(CLOCK_MONOTONIC, &tend);
    dummy = clock();                                                 // DUMMY
    printf("%f %ld", NANS_ELAPSED(tend, tstart) / num_pages, dummy); // DUMMY
    free(page_index);
    page_index = NULL;
    printf("%d ", pagemap->sum); // DUMMY
}

void page_fault_unmap(PageMap *pagemap)
{
    munmap(pagemap->base_ptr, pagemap->size);
    pagemap->base_ptr = NULL;
    pagemap->size = 0;
}

void shuffle(uint64_t *array, size_t n)
{

    srand(time(NULL));
    if (n > 1)
    {
        uint64_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}