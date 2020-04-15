#pragma once

#include "DijkstraAlgorithmBackend.h"
#include "mpi.h"

class DijkstraMPI
{
public:

	DijkstraMPI(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex, const std::vector<double>& graphData)
		: m_dijkstraBackend(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex), m_graphData(graphData) {}


	std::pair<const std::vector<double>, const std::vector<int>> run(MPI_Comm& communicator);

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	std::vector<double> m_graphData;

};
