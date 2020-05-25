#ifndef PAGE_FAULT_H
#define PAGE_FAULT_H
#include "../src/results.h"
#include "../src/functions.h"

typedef struct PageMap
{
    char *base_ptr;
    uint64_t size;
    int sum;
} PageMap;

void page_fault_lat_engine(Arguments *args);
void page_fault_setup(PageMap *memmap, Arguments *arg);
double page_fault_benchmark(PageMap *memmap, Results *results);
void page_fault_unmap(PageMap *pagemap);
void shuffle_page_index(uint64_t *array, size_t n);

#endif