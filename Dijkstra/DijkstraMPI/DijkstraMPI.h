#pragma once

#include "DijkstraAlgorithmBackend.h"
#include "mpi.h"

class DijkstraMPI
{
public:

	DijkstraMPI(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex, const std::vector<double>& graphData)
		: m_dijkstraBackend(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex), m_graphData(graphData) {}


	std::pair<const std::vector<double>, const std::vector<int>> run(MPI_Comm& communicator) {

		// while there are any vertices that have not been processed
		while (!m_dijkstraBackend.checkIfAllVerticesHaveBeenProcessed()) {

			// vertex that belongs to current node and is closest to cluster of already processed vertices
			VertexData localMin = m_dijkstraBackend.findVertexWithMinimalDistanceToCluster();

			// gather all local closest vertices and compute global one
			VertexData globalMin;
			MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, communicator);

			// safety check - if there was no valid closest vertex, we need to exit loop now
			if (globalMin.vertexNumber == -1) {
				break;
			}

			// mark vertex as processed
			m_dijkstraBackend.markVertexAsProcessed(globalMin.vertexNumber);

			// inner loop - computing new distances
			m_dijkstraBackend.performInnerForLoop(globalMin, m_graphData);
		}

		return std::make_pair(m_dijkstraBackend.getDistances(), m_dijkstraBackend.getPredecessors());
	}

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	std::vector<double> m_graphData;

};
