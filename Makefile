SHELL = /bin/sh
CC = g++
CFLAGS = -O2 -Wall -g
BM = Benchmark
BM_DEPS = src/main.cpp src/results.cpp engines/mmap.cpp engines/pmem.cpp src/parser.cpp src/functions.cpp
BM_Target = Benchmark
OBJ = obj/
OUT = out
LPMEM = -lpmem

.PHONY: all clean

all: $(BM)

$(BM): $(BM_DEPS)
	$(CC) $(CFLAGS) $^ $(LPMEM) -o $(BM_Target)

clean:
	$(RM) -r $(OBJ)* $(BM_Target) *.out