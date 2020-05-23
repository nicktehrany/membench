# mem_lat engine

## Overview

This Engine is meant to measure the latency of memory. It achieves this by creating a ring of pointers in a malloced buffer in memory, which are then walked along. Meaning, each pointer points to another random pointer within the buffer, starting from the initial pointer, it'll keep going to the next pointer 10,000,000 times. Time is measured for all 10,000,000 walks and the averaged.

## Usage

The engine takes several commands, which can be provided via cmd line or a file. Specify to run this engine as shown in the possible flags below. It is recommended to run multiple iterations, as each iteration will reinitialize the buffer, which will give more precise results over multiple runs.

## Possible Flags

```shell
-engine=mem_lat
-fsize= *specify the size of buffer to use for pointers given in K/M/G*
-iter= *Number of times to run the 10,000,000 walks*
```

## Results

The results will be shown in the results.out file in the root directory.

## Example run

Running the following command:

```shell
./Benchmark -engine=mem_lat -fsize=400M -iter=10
```

This would run the 10,000,000 pointer walks 10 times. The buffer of pointers will be 400MiB large.
