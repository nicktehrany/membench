# page_fault engine

This engine is meant to measure the cost of setting up a page table entry, along with I/O if file isn't in page cache. Initially, this was designed to compare dax-direct access for dax-enabled file systems on page faults, and compare it to page faults on regular file systems, which will do I/O to copy the file contents to page cache, whereas dax provides direct access to byte addressable persistent memory.

## Usage

## Example Run

