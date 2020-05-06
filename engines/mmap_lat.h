#ifndef MMAP_LAT_H
#define MMAP_LAT_H
#include "../src/results.h"
#include "../src/functions.h"

void mmap_lat_engine(Mapping *, Arguments *, Results *);
void mmap_lat_check_args(Arguments *);
void mmap_lat_prepare_mapping(Mapping *, Arguments);
void mmap_lat_cleanup_mapping(Mapping *);
void mmap_lat_prepare_map_anon(Mapping *mapping, uint64_t fsize);

#endif