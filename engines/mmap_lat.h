#ifndef MMAP_LAT_H
#define MMAP_LAT_H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_lat_engine(Mapping *, Arguments *, Results *);
void mmap_lat_check_args(Arguments *);
int mmap_lat_prep_file(Arguments args);

uint64_t mmap_lat_do_mmap(Mapping *, Arguments, int);
void mmap_lat_do_unmap(Mapping *);
uint64_t mmap_lat_do_mmap_anon(Mapping *mapping, uint64_t fsize, int fd);
void mmap_lat_cleanup_file(Mapping *mapping, int fd);

#endif