#include "pmem_cline.h"
#define PMEM_LEN 32768
#define CLINE_SIZE 64

// TODO Description for engine, README, docs
void pmem_cline_engine(Arguments *args)
{
    Pmem_Map pmem_map = {0, 0, 0};

    pmem_cline_prepare_mapping(args, &pmem_map);
    pmem_cline_benchmark(args, &pmem_map);
    remove("/mnt/mem/file");
}

void pmem_cline_prepare_mapping(Arguments *args, Pmem_Map *pmem_map)
{
    if ((pmem_map->addr = pmem_map_file("/mnt/mem/file", PMEM_LEN, PMEM_FILE_CREATE, 0666, &pmem_map->len, &pmem_map->is_pmem)) == NULL)
        LOG(ERROR, errno, "pmem_map");
}

void pmem_cline_benchmark(Arguments *args, Pmem_Map *pmem_map)
{

    char buf[PMEM_LEN] = {0};
    pmem_memcpy(pmem_map->addr, buf, sizeof(buf), PMEM_F_MEM_NOFLUSH); // Will flush manually later
    pmem_cline_flush_chunck(pmem_map);
    pmem_cline_flush_lines(pmem_map);

    //pmem_memcpy(pmem_map->addr, &buf, 64, 0);
}

// Flush consecutive chunks
uint64_t pmem_cline_flush_chunck(Pmem_Map *pmem_map)
{
    struct timespec tstart = {0, 0}, tend = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    int runs = PMEM_LEN / CLINE_SIZE / 5;
    for (int i = 0; i < runs; i++)
        pmem_persist(pmem_map->addr + i * CLINE_SIZE, CLINE_SIZE * 5);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    LOG(DEBUG, NANS_ELAPSED(tend, tstart) / runs, "CHUNK FLUSH");
}

// Flush individual lines
uint64_t pmem_cline_flush_lines(Pmem_Map *pmem_map)
{
    struct timespec tstart = {0, 0}, tend = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    int runs = PMEM_LEN / CLINE_SIZE / 5 / 10;
    for (int i = 0; i < runs; i++)
        pmem_persist(pmem_map->addr + i * CLINE_SIZE * 10, CLINE_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    LOG(DEBUG, NANS_ELAPSED(tend, tstart) / runs, "INDIVIDUAL FLUSH");
}