SHELL = /bin/sh
CC = g++
CFLAGS = -O2 -Wall
BM = Benchmark
BM_DEPS = src/main.cpp src/results.cpp src/functions.cpp
BM_Target = Benchmark
OBJ = obj/


.PHONY: all clean

all: $(BM)

$(BM): $(BM_DEPS)
	$(CC) $(CFLAGS) $^ -lpmem -o $(BM_Target)

clean:
	$(RM) -r $(OBJ)* $(BM_Target) *.out