CC=g++
CFLAGS=-Wall -O3
nsize=100

all:
	$(MAKE) --directory=generators/ nsize=$(nsize)
	mv generators/grid_engine_neighbours.hpp ./
	$(CC) $(CFLAGS) tests_of_correctness.cpp -o tests_of_correctness.exe
	./tests_of_correctness.exe

clean:
	rm -rf *.exe
