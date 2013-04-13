nsize=100

default:
	echo "namespace grid_engine {"              >  grid_engine_neighbours.hpp
	./neighbour_gen.py HexEven $(nsize) nodraw  >> grid_engine_neighbours.hpp
	./neighbour_gen.py HexOdd  $(nsize) nodraw  >> grid_engine_neighbours.hpp
	./neighbour_gen.py D8      $(nsize) nodraw  >> grid_engine_neighbours.hpp
	./neighbour_gen.py D4      $(nsize) nodraw  >> grid_engine_neighbours.hpp
	echo "}" >> grid_engine_neighbours.hpp
