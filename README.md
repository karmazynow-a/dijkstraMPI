# Dijkstra algorithm MPI

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