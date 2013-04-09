CC=g++
CFLAGS=-Wall -fopenmp -O3

all:
	$(MAKE) --directory=generators/
	./generators/neighbour_gen.exe hex 100 >  neighbours.h
	./generators/neighbour_gen.exe d8  100 >> neighbours.h
	./generators/neighbour_gen.exe d4  100 >> neighbours.h
	$(CC) $(CFLAGS) main.cpp -o main.exe

clean:
	rm -rf *.exe
