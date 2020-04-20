/**
*	@file DijkstraMPI.h
*	@brief This file contains MPI Dijkstra algorithm implementation.
*/

#pragma once

#include "DijkstraAlgorithmBackend.h"
#include "mpi.h"

/// <summary>
/// This class contains Dijkstra algorithm implementation created using MPI.
/// It uses DijkstraAlgorithmBackend class. 
/// </summary>
class DijkstraMPI
{
public:

	/// <summary>
	/// DijkstraMPI class constructor. Prepares all class properties (adjacency
	/// matrix chunk and algorithm backend).
	/// </summary>
	/// <param name="verticesToHandleRange">
	/// Range of vertices that should be handled by this object. Pair of integer 
	/// values a, b that represents range [a, b].
	/// </param>
	/// <param name="totalNumberOfVertices">
	/// Integer number that represents total number of vertices in processed 
	/// graph. 
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Integer number that represents index of source vertex. 
	/// </param>
	/// <param name="graphData">
	/// Part of adjacency matrix that is being processed by this object - it should be
	/// passed in a form of 1D vector of weights. Data from matrix should be aranged 
	/// column-wise.
	/// </param>
	DijkstraMPI(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex, const std::vector<double>& graphData)
		: m_dijkstraBackend(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex), m_graphData(graphData) {}


	/// <summary>
	/// Dijkstra algorithm implementation. Runs Dijkstra algorithm using MPI. Handles
	/// vertices that have been assigned to node that runs this function.
	/// </summary>
	/// <returns>
	/// Pair of vectors that represent Dijkstra algorithm results. First vector represents
	/// distances from source vertex, second vector contains predecessors.
	/// </returns>
	/// <param name="communicator">
	/// MPI communicator that allows communication between processes that take part in
	/// algorithm execution.
	/// </param>
	std::pair<const std::vector<double>, const std::vector<int>> run(MPI_Comm& communicator);

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	std::vector<double> m_graphData;

};
