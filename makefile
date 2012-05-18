CC=g++
CFLAGS=-Wall -fopenmp
PRE_FLAGS=-O3
ODIR=obj
DEPS = grid_engine.h

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(PRE_FLAGS) -c -o $@ $< $(CFLAGS)

grid_test: $(OBJ)
	$(CC) $(PRE_FLAGS) -o grid_test.exe $^ $(CFLAGS)
	du -hs ./grid_test.exe

debug: 

clean:
	rm -f $(ODIR)/*.o *~ core
