#ifndef MMAP_LAT_H
#define MMAP_LAT_H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_lat_engine(Arguments *args);
void mmap_lat_check_args(Arguments *args);
int mmap_lat_prep_file(Arguments args);

uint64_t mmap_lat_do_mmap(Mapping *mapping, Arguments args, int fd);
void mmap_lat_do_unmap(Mapping *mapping);
uint64_t mmap_lat_do_mmap_anon(Mapping *mapping, Arguments args, int fd);
void mmap_lat_cleanup_file(Mapping *mapping, int fd);

#endif