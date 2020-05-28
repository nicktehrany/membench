SHELL = /bin/sh
CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -pedantic-errors
BM = Benchmark
BM_DEPS = src/main.c src/results.c engines/mmap.c \
engines/mmap_lat.c engines/mem_lat.c engines/page_fault.c \
src/parser.c src/functions.c
BM_Target = Benchmark
OBJ = obj/
OUT = out

.PHONY: all clean

all: $(BM)

$(BM): $(BM_DEPS)
	$(CC) $(CFLAGS) $^ -o $(BM_Target)

clean:
	$(RM) -r $(OBJ)* $(BM_Target) *.out *.dat