SHELL = /bin/sh
CC = g++
CFLAGS = -O2 -Wall -g
BM = Benchmark
BM_DEPS = src/main.cpp src/results.cpp src/functions.cpp src/parser.cpp
BM_Target = Benchmark
OBJ = obj/
OUT = out


.PHONY: all clean

all: $(BM)

$(BM): $(BM_DEPS)
	$(CC) $(CFLAGS) $^ -lpmem -o $(BM_Target)

clean:
	$(RM) -r $(OBJ)* $(BM_Target) *.out