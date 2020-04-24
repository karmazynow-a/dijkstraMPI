CXX = g++
LIB_DIR = ../DijkstraCommon
LDFLAGS = -static -L$(LIB_DIR) -lDijkstraCommon

S_NAME = DijkstraSerial
S_HEADERS = \
	DijkstraSerial.h 

S_OBJ = \
	main.o \
    DijkstraSerial.o

LIB_HEADERS = \
	$(LIB_DIR)/DijkstraAlgorithmBackend.h 


$(MPI_NAME): $(S_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -I$(LIB_HEADERS)

%.o : %.cpp -I$(S_HEADERS) -I$(LIB_HEADERS)
	$(CXX) -o $@ $<

.PHONY: all clean

all: $(S_NAME) 

clean:
	rm -rf *.o $(S_NAME)