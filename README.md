# Dijkstra algorithm MPI

## Building 
For building the software you can use prepared `CMakeLists.txt`. The MPI 
library is necessary, and Doxygen for documentation.

Make options:

    make clean-all      cleans the directory from build files
    make doc            create Doxygen documentation
    make runMPI         runs MPI algorithm with default parameter VERTEX=0 
                            and FILE="../data/grap.dat"
    make runMPI VERTEX=V FILE=F
                        runs MPI algorithm with parameter VERTEX=V FILE=F
    make runSerial      runs serial algorithm with default parameter VERTEX=0
                            and FILE="../data/grap.dat"
    make runSerial VERTEX=V FILE=F
                        runs serial algorithm with parameter VERTEX=V FILE=F

## Generate sample data
Generate adjacency matrix representation of graph with given number of nodes
and vertices.

    generateGraphs.py [-h] [-e E] [-v V] [-f FILE]

    optional arguments:
    -h, --help            show this help message and exit
    -e E, --edges E       number of edges
    -v V, --vertices V    number of vertices
    -f FILE, --file FILE  name of output file

## Test the algorithm with sequential implementation

    test.py [-h] [-o FILE] FILE VERTEX

    positional arguments:
    FILE                  input file with adjacency matrix
    VERTEX                vertex to start algorithm

    optional arguments:
    -h, --help            show this help message and exit
    -o FILE, --output FILE
                            output file with path