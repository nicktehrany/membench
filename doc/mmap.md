# mmap engine

## Overview

This Engine is meant to measure the throughput to memory via reading and writing to/from a file mapped into memory. Engine will mmap a file of a given size, and continuously call memcpy, also for a given size, to read from it or write to it in sequential or random fashion, for the specified time.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It's possible to run mmap on any mount location, as well as using MAP_ANONYMOUS. It's also possible to measure the latency of mmap with MAP_POPULATE as a flag. Usage of these flags is shown below. It's possible to only run a certain number of memcpy calls, which can be specified via the -iter flag (Still have to specify a time), and runtime only includes memcpy calls. mmap pages can pre populates by setting the -map_pop flag to 1. If no directory is specified in the flag, the current directory will be used. The -init_mem flag will initialize the memory with random bytes for anonymous mappings (otherwise memory will be initialized with all 0s), this will also cause all pages to be mapped, therefore not recommended when measuring latencies of page faults. Init_mem flag will cause entire file to be in page cache, so if measuring page faults with device I/O, shouldn't be used.

## Possible Flags

```shell
-engine=mmap
-dir= *specify the location to mmap in (/dev/zero or /dev/null for MAP_ANONYMOUS)*
-fsize= *specify the size of file to mmap given as number with possible extensions M/G*
-copysize= *specify the size to memcpy at a time given as number with possible extension M/G*
-runtime= *specify time to run in seconds*
-mode= *read/write/randread/randwrite*
-tier= *specify number of times to call memcpy*
-map_pop= *0|1 to pass MAP_POPULATE to mmap*
-init_mem= *0|1 to initialize memory with random bytes instead of all 0s (Only for MAP_ANONYMOUS)*
-map_shared= *0|1 to specify to pass MAP_SHARED or MAP_PRIVATE to mmap (Default 0)*
```

## Results

The results will be shown in the results.out file. Results will display the directory used, flags used, how many iterations were run (how many times memcpy was called), the total runtime, the runtime of only memcpy calls, the specified file size, the specified copy size, the bandwidth for the mode, the total I/O amount, and minimum, maximum, and average latencies for memcpy calls. (These can go low if data comes from CPU caches)

## Example run

The following command:

```shell
./Benchmark -file=arguments.txt
```

with arguments.txt containing:

```shell
-runtime=4
-dir=/dev/zero
-fsize=1G
-copysize=4
-mode=randread
-engine=mmap
-map_pop=1
-map_shared=0
-iter=1000

```

(The empty line in the end of the file is important for parsing correctly!) This would run the engine for 1000 iterations of 4 byte memcpy, at random offsets, from the mapped file to a destination in memory. The file will be 1GiB large, and page table entries will be pre populated, as well as the mmap call getting flag MAP_PRIVATE. The runtime is set to a random number above 0, since 1000 memcpy calls will finish in a fraction of a section. (Runtime still needs to specified!)
