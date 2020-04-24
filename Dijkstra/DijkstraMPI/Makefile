MPICXX = mpicxx
LIB_DIR = ../DijkstraCommon
LDFLAGS = -static -L$(LIB_DIR) -lDijkstraCommon -L/usr/include/mpich/ 
MPI_LIB = -I/usr/include/mpich/

MPI_NAME = DijkstraMPI

MPI_OBJ = \
	DijkstraMPI.o \
	DijkstraMPISetup.o 

LIB_HEADERS = -I$(LIB_DIR)


$(MPI_NAME): $(MPI_OBJ)
	$(MPICXX) $< -o $@ $(LDFLAGS)

%.o: %.cpp 
	$(MPICXX) $(MPI_LIB) $(LIB_HEADERS) -c $< -o $@

.PHONY: all clean

all: $(MPI_NAME)

clean:
	rm -rf *.o $(MPI_NAME)