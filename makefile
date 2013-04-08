CC=g++
CFLAGS=-Wall -fopenmp -O3

all:
	$(MAKE) --directory=generators/
	./generators/d8_neighbour_gen.exe  100 >  neighbours.h
	./generators/hex_neighbour_gen.exe 100 >> neighbours.h
	$(CC) $(CFLAGS) main.cpp -o main.exe

clean:
	rm -rf *.exe
