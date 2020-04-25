CXX = g++
LIB_DIR = ../DijkstraCommon
LDFLAGS = -static -L$(LIB_DIR) -lDijkstraCommon

S_NAME = DijkstraSerial

S_OBJ = \
      main.o \
      DijkstraSerial.o

LIB_HEADERS = \
      -I$(LIB_DIR)/AdjacencyMatrix.h \
      -I$(LIB_DIR)/DijkstraAlgorithmBackend.h


$(S_NAME): $(S_OBJ)
	@$(CXX) -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	@$(CXX) -c $< -o $@ -I. -I$(LIB_DIR)

.PHONY: all clean

all: $(S_NAME) 

clean:
	@rm -rf *.o $(S_NAME)
