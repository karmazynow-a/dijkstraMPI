/** 
*	@mainpage Dijkstra algorithm implementations
*	Dijkstra algorithm implementations created using C++ and MPI.
*	@author Aleksandra Poreba 
*	@author Arkadiusz Kasprzak
*/

#pragma once

#include "VertexData.h"

#include <set>
#include <vector>


/// <summary>
/// This class contains Dijkstra algorithm backend - methods which are 
/// kind of building blocks that Dijkstra algorithm implementations can
/// then reuse. It manages distance and predecessors arrays as well as
/// stores cluster of already processed vertices - so user can focus on 
/// managing algorithm flow. Please note that this class is final - to 
/// use it, consider composition (not inheritance).
/// </summary>
class DijkstraAlgorithmBackend final {

public:

	/// <summary>
	/// Constructor of Dijkstra algorithm backend class. It prepares class
	/// properties - distance and predecessors arrays as well as cluster of
	/// already processed vertices. It initializes distance from source 
	/// vertex to itself to 0. 
	/// </summary>
	/// <param name="verticesToHandleRange">
	/// Pair of integer values that represent range of graph vertices this
	/// object should handle. Useful mainly in parallel implementations, in
	/// serial programs it should be just [0, totalNumberOfVertices - 1].
	/// </param>
	/// <param name="totalNumberOfVertices">
	/// Integer number that represents total number of vertices in processed 
	/// graph. 
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Integer number that represents index of source vertex. 
	/// </param>
	DijkstraAlgorithmBackend(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex);


	/// <summary>
	/// Checks if all graph vertices have already been processed - so it checks
	/// it size of cluster is equal to total number of verices in input graph.
	/// </summary>
	/// <returns>
	/// Boolean that indicates if all vertices have been processed. 
	/// </returns>
	inline bool checkIfAllVerticesHaveBeenProcessed() const {
		return processedVerticesCluster.size() == totalNumberOfVertices;
	}
	

	/// <summary>
	/// Marks vertiex as processed - adds it to cluster of processed vertices.
	/// </summary>
	/// <param name="vertex">
	/// Number of vertex that should be added to the cluster of processed vertices.
	/// </param>
	inline void markVertexAsProcessed(int vertex) {
		processedVerticesCluster.insert(vertex);
	}
	

	/// <summary>
	/// This method finds vertex with minimal distance to cluster of processed
	/// vertices. 
	/// </summary>
	/// <returns>
	/// VertexData structure with vertex index and distance. If no proper vertex has
	/// been found, returns VertexData with vertex index set to -1 and distance to 
	/// infinity. 
	/// </returns>
	VertexData findVertexWithMinimalDistanceToCluster();


	/// <summary>
	/// This method performs inner for loop of dijksta algorithm - it calculates 
	/// distances and predecessors for all vertices that have not been already
	/// processed using vertex that has been recently added to cluster (the one
	/// that was closest to it).
	/// </summary>
	/// <param name="vertexClosestToCluster">
	/// VertexData structure that represents vertex that  has been recently added
	/// to cluster (so the one with shortest distance to the cluster).
	/// </param>
	/// <param name="processedPartOfAdjacencyMatrix">
	/// Part of adjacency matrix that is being processed by this object - it should be
	/// passed in a form of 1D vector of weights. Data from matrix should be aranged 
	/// column-wise.
	/// </param>
	void performInnerForLoop(const VertexData& vertexClosestToCluster, const std::vector<double>& processedPartOfAdjacencyMatrix);


	/// <summary>
	/// Simple getter method that returns reference to const vector of distances. 
	/// </summary>
	/// <returns>
	/// Reference to const vector of distances.Please note that it is a reference
	/// so it is valid only as long as this object is valid.
	/// </returns>
	const std::vector<double>& getDistances() const {
		return distances;
	}


	/// <summary>
	/// Simple getter method that returns reference to const vector of predecessors. 
	/// </summary>
	/// <returns>
	/// Reference to const vector of predecessors. Please note that it is a reference
	/// so it is valid only as long as this object is valid.
	/// </returns>
	const std::vector<int>& getPredecessors() const {
		return predecessors;
	}

private:

	std::pair<int, int> verticesToHandleRange;
	std::vector<double> distances;
	std::vector<int> predecessors;
	std::set<int> processedVerticesCluster;

	int totalNumberOfVertices;

};