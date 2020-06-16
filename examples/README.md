# Workloads

These are some small example workloads, which can be invoked with the following command (All arguments can also be individually supplied via cmd line):

```shell
./Benchmark -file=examples/example1.txt
```

## Example 1

This example will use the mmap engine to call mmap with MAP_ANONYMOUS MAP_POPULATE and MAP_PRIVATE as flags. The mapping will be 3GiB large, and the engine will continuously for 5 seconds memcpy 4KiB, at a time, from random offsets in the mapping to memory (Reading from the mapping 4KiB at a time). This could be used to measure bandwidth.

## Example 2

This example will use the mmap_lat engine, to call mmap on the /mnt/mem/file (The file needs to already be there, see docs for help on how that's done easily). It will then map the file into memory with the MAP_SHARED flag and pre populate page table entries, unmap the file, map it again, and repeat this 100 times. This is meant to measure the cost of mmap calls with pre populating page table entries. (This is merely an example and larger iterations should be uses, as well as depending on the file size users should also adjust the iterations)

## Example 3

This example also uses the mmap engine. It will map /mnt/mem/file into memory and read 4 bytes, at a time, from it at random offsets, this is repeated 1,000 times, then the file will be unmapped, mapped again, and the same is repeated. The mapping, reading, unmapping is repeated 1,000 times. Runtime is specified at 50 seconds, so if execution takes longer than that, it'll stop, and give results, otherwise it'll stop once it finished the 1,000 iterations. This example could be used as an indication for page fault cost times, including fs I/O if file isn't in page cache.
