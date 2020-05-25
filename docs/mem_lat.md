# mem_lat engine

## Overview

This Engine is meant to measure the latency of memory. It achieves this by creating a ring of pointers in a malloced buffer in memory, which are then walked along. Allocation works by starting with the base pointer, setting it to point to some other random address in the buffer, then the pointer at that address will be set to point to some other random address, and so on. The last pointer to be set will point back to the base pointer, therefore not every pointer in the buffer may point to an address, but this will create a cycle that can be walked multiple times. The walk will start at the base pointer and it'll keep going to the next pointer 10,000,000 times.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It is recommended to run multiple iterations, as each iteration will reinitialize the buffer, which will give more precise results over multiple runs.

## Possible Flags

```shell
-engine=mem_lat
-size= *specify the size of buffer to use for pointers given in K/M/G*
-iter= *Number of times to run the engine*
```

## Results

The results will be shown in the results.out file in the root directory.

## Example run

Running the following command:

```shell
./Benchmark -engine=mem_lat -size=400M -iter=10
```

This would run the engine 10 times (10,000,000 pointer walks each time). The buffer of pointers will be 400MiB large.
