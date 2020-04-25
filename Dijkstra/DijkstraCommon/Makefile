CXX=g++

LIB_NAME = libDijkstraCommon.a
LIB_HEADERS = \
        AdjacencyMatrix.h \
        CommandLineArgumentsExtractor.h \
        DijkstraAlgorithmBackend.h \
        Log.h \
        ResultsPrinter.h \
        Validator.h \
        VertexData.h 

LIB_OBJ = \
        DijkstraAlgorithmBackend.o \
	AdjacencyMatrix.o \
	ResultsPrinter.o \
	Validator.o


$(LIB_NAME): $(LIB_OBJ)
	@ar crf $(LIB_NAME) $^

%.o : %.cpp
	@$(CXX) -c $< -o $@

$(LIB_OBJ) : $(LIB_HEADERS)

.PHONY: all clean

all: $(LIB_NAME) 

clean:
	@rm -rf *.o $(LIB_NAME)
