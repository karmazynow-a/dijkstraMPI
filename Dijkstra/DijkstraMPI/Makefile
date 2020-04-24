MPICXX = mpixx
LIB_DIR = ../DijkstraCommon
LDFLAGS = -static -L$(LIB_DIR) -lDijkstraCommon -L/usr/include/mpich/ 
MPI_LIB = /usr/include/mpich/mpi.h

MPI_NAME = DijkstraMPI
MPI_HEADERS = \
	DijkstraMPI.h \
	DijkstraMPISetup.h

MPI_OBJ = \
	main.o \
    DijkstraMPI.o \
    DijkstraMPISetup.o 

LIB_HEADERS = \
	$(LIB_DIR)/AdjacencyMatrix.h \
	$(LIB_DIR)/DijkstraAlgorithmBackend.h


$(MPI_NAME): $(MPI_OBJ)
	$(MPICXX) -o $@ $^ $(LDFLAGS)

%.o : %.cpp -I$(MPI_LIB) -I$(MPI_HEADERS) -I$(LIB_HEADERS)
	$(MPICXX) -c $< -o $@

.PHONY: all clean

all: $(MPI_NAME) 

clean:
	rm -rf *.o $(MPI_NAME)