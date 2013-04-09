CC = g++
CFLAGS = -O3

default:
	$(CC) $(CFLAGS) neighbour_gen.cpp -o neighbour_gen.exe
	echo "namespace grid_engine {" > neighbours.h
	./neighbour_gen.exe hex 100 >>  neighbours.h
	./neighbour_gen.exe d8  100 >> neighbours.h
	./neighbour_gen.exe d4  100 >> neighbours.h
	echo "}" >> neighbours.h

clean:
	rm -f *exe
