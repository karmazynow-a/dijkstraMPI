#!/usr/bin/env python
# script for generating graphs with given number of nodes
# if size is not given the default value is 10

import argparse
import numpy as np
import warnings

parser = argparse.ArgumentParser(description = 
	'Generate adjacency matrix representation of graph with given number of nodes and vertices.')
parser.add_argument('-e', '--edges', metavar='E', type=int, nargs=1, 
	default=[10], help='number of edges')
parser.add_argument('-v', '--vertices', metavar='V', type=int, nargs=1, 
	default=[10], help='number of vertices')
parser.add_argument('-f', '--file', metavar='FILE', type=str, nargs=1, 
	default=['graph.dat'], help='name of output file')

args = parser.parse_args()

def main():
	e = args.edges[0] 		# number of edges
	v = args.vertices[0] 	# number of vertices
	if e > v*(v-1)/2:
		warnings.warn('Maximum number of edges for {0} verticles is {1}'.format(v, v*(v-1)/2))
		e = np.random.randint(low = 0, high = v*(v-1)/2)
		warnings.warn('Changing the number of edges to {0}'.format(e))

	adj = np.zeros((v, v)) 	# adjacency matrix
	
	i = 0					# number of assigned edges
	while i < e:
		n1, n2 = np.random.randint(low = 0, high = v, size = 2)
		if n1 != n2 and adj[n1][n2] == 0:
			adj[n1][n2] = adj[n2][n1] \
				= round(np.random.uniform(low = 0, high = 10), 2)
			i+=1

	saveToFile(matrix = adj)

	
def saveToFile(matrix):
	fileName = args.file[0]
	with open(fileName, 'w') as file:
		file.write("{0}\n".format(len(matrix)))
		# cut the matrix formatting
		matFormat = '\n'.join(['  '.join(['{:.4f}'.format(item) for item in row]) for row in matrix])
		file.write(matFormat)

if __name__ == '__main__':
    main()

