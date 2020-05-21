# Benchmarking Memory Performance

## Overview

This Benchmark was originally created to investigate performance of DAX enabled file systems in persistent memory. It comes with two engines, mmap, which measures memory throughput, and mmap_lat, which measures mmap latencies.

## Directory Setup

The project has the following directories:

* doc (Contains documentation for each engine)
* engines (Contains the code for each engine)
* scripts (Contains script used to run benchmarks. Details are in the scripts README)
* src (Contains main code for the Benchmark)
* workloads (Pre-defined workloads that are run form the scripts)

## Usage

Running make from the root directory will compile the program and create an executable called Benchmark in the root directory. Information about each engine and how to use them is in their respective file in the documentation.
