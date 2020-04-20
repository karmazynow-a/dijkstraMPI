/**
*	@file DijkstraSerial.h
*	@brief This file contains serial Dijkstra algorithm implementation.
*/


#pragma once

#include "DijkstraAlgorithmBackend.h"

/// <summary>
/// This class contains serial Dijkstra algorithm implementation.
/// It uses DijkstraAlgorithmBackend class. 
/// </summary>
class DijkstraSerial
{
public:

	/// <summary>
	/// DijkstraSerial class constructor. Prepares all class properties (adjacency
	/// matrix chunk and algorithm backend).
	/// </summary>
	/// <param name="totalNumberOfVertices">
	/// Integer number that represents total number of vertices in processed 
	/// graph. 
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Integer number that represents index of source vertex. 
	/// </param>
	/// <param name="graphData">
	/// Adjacency matrix that is being processed by this object - it should be
	/// passed in a form of 1D vector of weights. Data from matrix should be aranged 
	/// column-wise.
	/// </param>
	DijkstraSerial(int totalNumberOfVertices, int sourceVertexIndex, const std::vector<double>& graphData) 
		: m_dijkstraBackend(std::make_pair(0, totalNumberOfVertices - 1), totalNumberOfVertices, sourceVertexIndex), m_graphData(graphData) {}


	/// <summary>
	/// Dijkstra algorithm implementation. Runs serial implementation of Dijkstra algorithm. 
	/// Handles all graph vertices.
	/// </summary>
	/// <returns>
	/// Pair of vectors that represent Dijkstra algorithm results. First vector represents
	/// distances from source vertex, second vector contains predecessors.
	/// </returns>
	std::pair<const std::vector<double>, const std::vector<int>> run();

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	std::vector<double> m_graphData;
};

