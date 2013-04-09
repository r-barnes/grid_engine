CC=g++
CFLAGS=-Wall -fopenmp -O3

all:
	$(MAKE) --directory=generators/
	mv generators/neighbours.h ./
	$(CC) $(CFLAGS) main.cpp -o main.exe

clean:
	rm -rf *.exe
