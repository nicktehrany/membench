# mem_lat engine

## Overview

This Engine is meant to measure the latency of memory. It achieves this by creating a ring of pointers in a malloced buffer in memory, which are then walked along. Meaning, each pointer points to another random pointer within the buffer, starting from the initial pointer, it'll keep going to the next pointer (which would be considered 1 iteration). Time is measured for the given iterations and then divided by the iterations.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It is recommended to leave iterations at default, since lower iterations won't give accurate results.

## Possible Flags

```shell
-engine=mem_lat
-fsize= *specify the size of buffer to use for pointers given in K/M/G*
-map_pop= *0|1 to pass MAP_POPULATE to mmap call (Default 0)*
-iter= *Number of times to walk to the next pointer (Default 10,000,000)*
```

## Results

The results will be shown in the results.out file in the root directory.

## Example run

Running the following command:

```shell
./Benchmark -engine=mem_lat -fsize=400M
```

This would run the engine for 10,000,000 (Default) iterations of pointer walking. The buffer of pointers will be 400MiB large.
