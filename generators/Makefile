CC = g++
CFLAGS = -O3

default:
	$(CC) $(CFLAGS) neighbour_gen.cpp -o neighbour_gen.exe
	echo "namespace grid_engine {"      >  grid_engine_neighbours.h
	./neighbour_gen.exe Hex $(nsize)    >> grid_engine_neighbours.h
	./neighbour_gen.exe D8  $(nsize)    >> grid_engine_neighbours.h
	./neighbour_gen.exe D4  $(nsize)    >> grid_engine_neighbours.h
	echo "}" >> grid_engine_neighbours.h

clean:
	rm -f *exe
