CC=g++
CFLAGS=-I/eigen-3.3.7 -O3 -fopenmp

DEPS = DCPSEgeneral.hpp functions.hpp MatrixOperation.hpp neighbourfindsimple.hpp readgeom.hpp readparameter.hpp
OBJ = main.o DCPSEgeneral.o functions.o MatrixOperation.o neighbourfindsimple.o readgeom.o readparameter.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

DCPSE_SOLID_2D_Solver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o DCPSE_SOLID_2D_Solver