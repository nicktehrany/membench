# Workloads

These workloads were used to benchmark memory performance and page fault cost. These were mainly invoked by scripts, but can just as easily be invoked by a terminal command like:

```shell
./Benchmark -file=workloads/mmap_mem_lat.txt
```

## mmap_mem_lat.txt

The mmap_mem_lat.txt will measure memory latencies by using the mmap engine. To be sure about the memory latency, I also created the mem_lat engine (It's better to be able to prove your values multiple ways). The script will simply mmmap a 3GiB file, pre populate page table entries to avoid page faults, and memcpy 10,000 times at random offsets (memcpy 4 bytes, since that's a word).

## page_faults

These scripts are meant to measure page fault latencies for file systems, as well as MAP_ANONYMOUS. They give arguments to have a 3GiB file mapped into memory, with MAP_PRIVATE, for /mnt/mem (mounted file system) and MAP_ANONYMOUS, and call memcpy for 4 bytes 1,000 times at random offsets. Since page table entries aren't pre populated, the accesses will result in page faults, and file system I/O to copy the file to page cache.
