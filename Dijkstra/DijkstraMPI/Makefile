MPICXX = mpicxx
LIB_DIR = ../DijkstraCommon
LINK_FLAGS = -static -L$(LIB_DIR) -lDijkstraCommon -L/usr/include/mpich/ 
MPI_LIB = /usr/include/mpich/mpi.h

ALG_NAME = DijkstraMPI
ALG_HEADERS = \
	DijkstraMPI.h \
	DijkstraMPISetup.h

ALG_OBJ = \
	main.o \
    DijkstraMPI.o \
    DijkstraMPISetup.o 

LIB_HEADERS = \
	$(LIB_DIR)/AdjacencyMatrix.h \
	$(LIB_DIR)/DijkstraAlgorithmBackend.h


$(ALG_NAME): $(ALG_OBJ)
	$(MPICXX) -o $@ $^ $(LINK_FLAGS)

%.o : %.cpp -I$(ALG_LIB) -I$(ALG_HEADERS) -I$(LIB_HEADERS) 
	$(MPICXX) -c $< -o $@ 

.PHONY: all clean

all: $(ALG_NAME) 

clean:
	rm -rf *.o $(ALG_NAME)