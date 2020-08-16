# Benchmarking Memory and File System Performance

## Overview

This benchmark was originally created to investigate performance of DAX enabled file systems in persistent memory. It was also used to get a baseline for memory bandwidth and latency, as well as benchmarking non DAX-enabled file systems, that are running on emulated block devices in memory. It includes four engines, mmap, which measures bandwidth and latency of accessing memory mapped files, mmap_lat, which measures latencies of mmap calls, mem_lat, which measures latency for accessing memory, and page_fault, which measures latency of hitting page faults on a memory mapped file.

## Directory Setup

The project has the following directories:

* docs (Contains documentation for each engine)
* engines (Contains the code for each engine)
* examples (Pre-defined example workloads)
* src (Contains main code for the Benchmark)
* workloads (workloads used for the thesis)

## Usage

Running ```make``` from the root directory will compile the program and create an executable called membench in the root directory. Information about each engine and how to use them is in their respective file in the documentation. Executable can then be run using:

```sh
./membench -help
```
