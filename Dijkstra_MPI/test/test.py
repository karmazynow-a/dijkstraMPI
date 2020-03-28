#!/usr/bin/env python
# sequantial implementation of djikstra algorithm
# script for testing paraller djikstra algorithm

import argparse
import numpy as np

parser = argparse.ArgumentParser(description = 
	'Djikstra algorithm')
parser.add_argument('inputFile', metavar='FILE', type=str, nargs=1, 
	help='input file with adjacency matrix')
parser.add_argument('startVertex', metavar='VERTEX', type=int, nargs=1, 
	help='vertex to start algorithm')
parser.add_argument('-o', '--output', metavar='FILE', type=str, nargs=1, 
	default=['output.dat'], help='output file with path')

args = parser.parse_args()


def main():
	V, E, w = readData(args.inputFile[0])
	weights, paths = djikstra(V, E, w, args.startVertex[0])
	saveToFile(weights, paths, args.output[0])


# V - list of all vertices
# E - egdes
# w - weights
# s - start vertex
def djikstra(V, E, w, s):

	# find vertex with minimum d that is also in set T
	def findMin():
		vmin = list(T)[0]
		for v in T:
			if d[v] < d[vmin]:
				vmin = v
		return vmin

	# find neighbours of vertex v that are also in set T
	def findNeighbours(v):
		n = set()
		for edge in E:
			if v in edge and edge.issubset(T):
				tmp = [v2 for v2 in edge if v2 != v]
				n.add( tmp[0] )
		return n

	# d - shortest known path from v_i to v_s
	wMax = np.inf
	d = { v:wMax for v in V }
	d[s] = 0
	dPaths = { v:[] for v in V }
	T = V # set of verticles for which shortest paths have not been found yet
	for i in range(0, len(V)):
		vm = findMin()
		for n in findNeighbours(vm):
			# vertices can be stored in two combinations [n, vm] or [vm, n]
			if d[n] > d[vm] + w[ E.index(set([n, vm])) ]:
				d[n] = d[vm] + w[ E.index(set([n, vm])) ]
				dPaths[n] = dPaths[vm] + [vm]
			elif d[n] > d[vm] + w[ E.index(set([vm, n])) ]:
				d[n] = d[vm] + w[ E.index(set([vm, n])) ]
				dPaths[n] = dPaths[vm] + [vm]

		T.remove(vm)

	return d, dPaths


# save data to file in format: vertex path_weight path_verticles
def saveToFile(weights, paths, fileName):
	with open(fileName, 'w') as file:
		output = "vertex path_weight path_verticles\n"
		for k in weights.keys():
			output += "{0} {1} {2} \n".format(k, weights[k], paths[k]) 
		file.write( output )


# read data from file with adjacency matrix
# and organize it into verticles, edges and weights
def readData(fileName):
	mat = [] #adjacency matrix
	with open(fileName, 'r') as file:
		file.readline() # skip first line with numbers of v
		for line in file.readlines():
			mat.append( [ float(i) for i in line.strip('\n').split(' ') if i ] )

	V = [ i for i in range (0, len(mat) )]
	E = []
	w = []
	for i in range(0, len(mat) ):
		for j in range(0, i):
			if mat[i][j] > 0:
				E.append(set([i, j]))
				w.append(mat[i][j])

	return V, E, w


if __name__ == '__main__':
    main()

