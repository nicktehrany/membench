# mmap_lat engine

## Overview

This Engine is meant to measure the latencies of mmap calls. It achieves this by continuously calling mmap and unmap and measures the latency of each mmap.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It's possible to run mmap on any mount location, as well as using MAP_ANONYMOUS. It's also possible to measure the latency of mmap with MAP_POPULATE as a flag. Usage of these flags is shown below.

## Possible Flags

```shell
-engine=mmap_lat
-dir= *specify the location to mmap in (/dev/zero or /dev/null for MAP_ANONYMOUS)*
-fsize= *specify the size of file to mmap given in B/M/G*
-map_pop= *0|1 to pass MAP_POPULATE to mmap call (Default 0)*
-iter= *Number of iterations to run mmap for (Default 1000)*
```
