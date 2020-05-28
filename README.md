# Benchmarking DAX Performance

## Overview

This Benchmark was originally created to investigate performance of DAX enabled file systems in persistent memory. It was also used to get a baseline for memory bandwidth and latency, as well as benchmarking non-dax enabled file systems, that are running on emulated block devices in memory. It includes three engines, mmap, which measures memory throughput, mmap_lat, which measures mmap latencies, mem_lat, which measures memory latencies, and page_fault, which measures page fault latencies. I included README files in most directories to provide as much info as possible about what and how I ran the different benchmarks.

## Directory Setup

The project has the following directories:

* docs (Contains documentation for each engine)
* examples (Pre-defined example workloads)
* engines (Contains the code for each engine)
* src (Contains main code for the Benchmark)

## Usage

Running ```make``` from the root directory will compile the program and create an executable called Benchmark in the root directory. Information about each engine and how to use them is in their respective file in the documentation.

mmap and mmap_lat engines need an already existing file, therefore if no file exists, quickly create a file and fill it with random bytes using these commands:

```shell
touch /mnt/mem/file
dd if=/dev/urandom of=/mnt/mem/file bs=100M count=8
```
