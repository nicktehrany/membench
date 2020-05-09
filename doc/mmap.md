# mmap engine

## Overview

This Engine is meant to measure the throughput to memory via reading and writing to/from a file mapped into memory. Engine will mmap a file of a given size, and continuously call memcpy, also for a given size, to read from it or write to it in sequential or random fashion, for the specified time.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It's possible to run mmap on any mount location, as well as using MAP_ANONYMOUS. It's also possible to measure the latency of mmap with MAP_POPULATE as a flag. Usage of these flags is shown below. It's possible to only run a certain number of memcpy calls, which can be specified via the -iter flag (Still have to specify a time). mmap pages can pre populates by setting the -map_pop flag to 1. If no directory is specified in the flag, the current directory will be used.

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
```
