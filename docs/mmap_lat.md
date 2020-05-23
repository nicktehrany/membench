# mmap_lat engine

## Overview

This Engine is meant to measure the latencies of mmap calls. It achieves this by continuously calling mmap and unmap and measures the latency of each mmap.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It's possible to run mmap on any mount location, as well as using MAP_ANONYMOUS. It's also possible to measure the latency of mmap with MAP_POPULATE as a flag. If no directory is specified in the flag, the current directory will be used. Runtime is set to 1 second by default.

## Possible Flags

```shell
-engine=mmap_lat
-dir= *specify the location to mmap in (/dev/zero or /dev/null for MAP_ANONYMOUS)*
-fsize= *specify the size of file to mmap given in K/M/G*
-map_pop= *0|1 to pass MAP_POPULATE to mmap call (Default 0)*
-iter= *Number of iterations to run mmap for*
-map_shared= *0|1 to specify to pass MAP_SHARED or MAP_PRIVATE to mmap (Default 0/MAP_PRIVATE)*
```

## Results

The results will be shown in the results.out file in the root directory. Results will display the directory used, flags used, how many iterations were run (how many times mmap was called), the total runtime, the specified file size, minimum, maximum, and average latencies for the mmap calls.

## Example run

Running the following command:

```shell
./Benchmark -file=arguments.txt
```

with arguments.txt containing:

```shell
-runtime=40
-engine=mmap_lat
-dir=/mnt/mem
-fsize=1G
-map_pop=0
-iter=100000
-map_shared=0

```

This would run the engine for 100,000 iterations of mmap. The file will be 1GiB large, and page table entries will not be pre populated, as well as the mmap call getting flag MAP_PRIVATE. The runtime will be 1 by default, but in case the run will take longer than 1 second, it's good to specify a large enough random runtime.
