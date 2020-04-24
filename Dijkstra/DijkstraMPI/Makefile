MPICXX = mpicxx
LIB_DIR = ../DijkstraCommon
LDFLAGS = -static -L$(LIB_DIR) -lDijkstraCommon -L/usr/include/mpich/
MPI_LIB = -I/usr/include/mpich/

ALG_NAME = DijkstraMPI

ALG_OBJ = \
	main.o \
	DijkstraMPI.o \
	DijkstraMPISetup.o 

LIB_HEADERS = -I$(LIB_DIR)


$(ALG_NAME): $(ALG_OBJ)
	$(MPICXX) $< -o $@ $(LDFLAGS)

%.o: %.cpp 
	$(MPICXX) $(ALG_LIB) $(LIB_HEADERS) -c $< -o $@

.PHONY: all clean

all: $(ALG_NAME)

clean:
	rm -rf *.o $(ALG_NAME)