# Benchmarking Memory Performance

## Overview

This Benchmark was originally created to investigate performance of DAX enabled file systems in persistent memory. It includes three engines, mmap, which measures memory throughput, mmap_lat, which measures mmap latencies, and mem_lat, which measures memory latencies. I included README files in most directories to provide as much info as possible about what and how I ran the different benchmarks.

## Directory Setup

The project has the following directories:

* docs (Contains documentation for each engine)
* engines (Contains the code for each engine)
* scripts (Contains script used to run benchmarks. Details are in the scripts README)
* src (Contains main code for the Benchmark)
* workloads (Pre-defined workloads that are run form the scripts)

## Usage

Running make from the root directory will compile the program and create an executable called Benchmark in the root directory. Information about each engine and how to use them is in their respective file in the documentation.
