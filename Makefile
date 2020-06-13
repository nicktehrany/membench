SHELL = /bin/sh
CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -pedantic-errors
FULL_DEPS = src/main.c src/results.c engines/mmap.c \
engines/mmap_lat.c engines/mem_lat.c engines/page_fault.c \
src/parser.c src/functions.c
FULL_DEPS_H = src/results.h engines/mmap.h engines/mmap_lat.h \
engines/mem_lat.h engines/page_fault.h src/parser.h src/functions.h
BASIC_DEPS = src/main.c src/results.c engines/mmap.c \
engines/mmap_lat.c engines/mem_lat.c engines/page_fault.c \
src/parser.c src/functions.c
BASIC_DEPS_H = src/results.h engines/mmap.h engines/mmap_lat.h \
engines/mem_lat.h engines/page_fault.h src/parser.h src/functions.h
BM_Target = Benchmark
OUT = out

.PHONY: all clean

all: full

full: $(FULL_DEPS) $(FULL_DEPS_H)
	@echo "Compiling Full Verison"
	$(CC) $(CFLAGS) -DPMEM $^ -o $(BM_Target)

basic: $(BASIC_DEPS) $(BASIC_DEPS_H)
	@echo "Compiling Basic Version without PMEM"
	$(CC) $(CFLAGS) $^ -o $(BM_Target)

clean:
	$(RM) -r $(BM_Target) *.out *.dat