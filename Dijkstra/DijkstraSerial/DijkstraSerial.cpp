#include "DijkstraSerial.h"


std::pair<const std::vector<double>, const std::vector<int>> DijkstraSerial::run() {

	// while there are any vertices that have not been processed
	while (!m_dijkstraBackend.checkIfAllVerticesHaveBeenProcessed()) {

		// vertex that is closest to cluster of already processed vertices
		VertexData vertexWithMinDistanceToCluster = m_dijkstraBackend.findVertexWithMinimalDistanceToCluster();

		// safety check - if there was no valid closest vertex, we need to exit loop now
		if (vertexWithMinDistanceToCluster.vertexNumber == -1) {
			break;
		}

		// mark vertex as processed
		m_dijkstraBackend.markVertexAsProcessed(vertexWithMinDistanceToCluster.vertexNumber);

		// inner loop - computing new distances
		m_dijkstraBackend.performInnerForLoop(vertexWithMinDistanceToCluster, m_graphData);
	}

	return std::make_pair(m_dijkstraBackend.getDistances(), m_dijkstraBackend.getPredecessors());
}