#include "mmap_lat.h"
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

void mmap_lat_engine(Mapping *mapping, Arguments *args, Results *results)
{
    //sleep(6);
    //mmap_check_args(args);
    mmap_lat_prepare_mapping(mapping, *args);
    mmap_lat_cleanup_mapping(mapping);
    dump_results(*results, *args);
}

void mmap_lat_prepare_mapping(Mapping *mapping, Arguments args)
{
    if (args.map_anon)
        mmap_lat_prepare_map_anon(mapping, args.fsize);
    else
    {
        int fd;
        if ((fd = open(args.path, O_CREAT | O_RDWR, 0666)) < 0)
        {
            perror("File Open");
            exit(1);
        }

        if (ftruncate(fd, args.fsize) != 0)
        {
            perror("truncate");
            close(fd);
            remove(args.path);
            exit(1);
        }

        if ((mapping->addr = (char *)mmap(0, args.fsize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            remove(args.path);
            exit(1);
        }

        close(fd);

        mapping->is_pmem = 0;
        mapping->map_anon = 0;
        mapping->fsize = args.fsize;
        mapping->fpath = args.path;
    }
}

// // Mapping is anonymous
void mmap_lat_prepare_map_anon(Mapping *mapping, uint64_t fsize)
{
    // MAP_ANONYMOUS not backed by file on file system
    if ((mapping->addr = (char *)mmap(0, fsize, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    mapping->is_pmem = 0;
    mapping->map_anon = 1;
    mapping->fsize = fsize;
}

void mmap_lat_cleanup_mapping(Mapping *mapping)
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
    mapping->is_pmem = 0;
    mapping->map_anon = 0;
}