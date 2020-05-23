# mmap engine

## Overview

This Engine is meant to measure the throughput to memory via reading and writing to/from a file mapped into memory. Engine will mmap a file of a given size, and continuously call memcpy, for a given size, to read from it or write to it in sequential or random fashion, for the specified time. If a the given memcpy size is smaller than the PAGE_SIZE, the file will be divided into PAGE_SIZE chunks and memcpy will be called on each chunk once. For example, a copy size of 256 bytes is give and a sequential read is done, then on the first read the first 256 bytes of the file will be memcpy into a buffer in memory. The next memcpy will not be on the 256 bytes following the just copied bytes, but rather the first 256 bytes of the next PAGE_SIZE chunk.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. All possible flags are show below. If no directory is specified in the flag, the current directory will be used. Runtime is set to 1 second by default.

## Possible Flags

```shell
-engine=mmap
-dir= *specify the location to mmap in (/dev/zero or /dev/null for MAP_ANONYMOUS)*
-fsize= *specify the size of file to mmap given as number with possible extensions K/M/G*
-copysize= *specify the size to memcpy at a time given as number with possible extension K/M/G*
-runtime= *specify time to run in seconds*
-mode= *read/write/randread/randwrite*
-iter= *specify number of times to call memcpy*
-map_pop= *0|1 to pass MAP_POPULATE to mmap*
-map_shared= *0|1 to specify to pass MAP_SHARED or MAP_PRIVATE to mmap (Default 0/MAP_PRIVATE)*
```

## Results

The results will be shown in the results.out file in the root directory. Results will display the directory used, flags used, how many iterations were run (how many times memcpy was called), the total runtime, the runtime of only memcpy calls, the specified file size, the specified copy size, the bandwidth per second, the total I/O amount, and minimum, maximum, and average latencies for memcpy calls. (These can drop to a few nanoseconds if data comes from CPU caches)

## Example run

Running the following command:

```shell
./Benchmark -file=arguments.txt
```

with arguments.txt containing:

```shell
-engine=mmap
-runtime=4
-dir=/dev/zero
-fsize=1G
-copysize=4
-mode=randread
-map_pop=1
-map_shared=0
-iter=1000

```

This would run the engine for 1,000 iterations of 4 byte memcpy, at random offsets, from the mapped file to a destination in memory. The file will be 1GiB large, and page table entries will be pre populated, as well as the mmap call getting flag MAP_PRIVATE. The runtime will be 1 by default, but in case the run will take longer than 1 second, it's good to specify a large enough random runtime.
