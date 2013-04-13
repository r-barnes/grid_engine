CC=g++
CFLAGS=-Wall -fopenmp -O3
nsize=100

all:
	$(MAKE) --directory=generators/ nsize=$(nsize)
	mv generators/grid_engine_neighbours.hpp ./
	$(CC) $(CFLAGS) tests_of_correctness.cpp -o tests_of_correctness.exe
	./tests_of_correctness.exe
	$(CC) $(CFLAGS) main.cpp -o main.exe

clean:
	rm -rf *.exe
