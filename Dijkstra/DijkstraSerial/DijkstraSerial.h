#pragma once

#include "DijkstraAlgorithmBackend.h"

class DijkstraSerial
{
public:
	DijkstraSerial(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex, const std::vector<double>& graphData) 
		: m_dijkstraBackend(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex), m_graphData(graphData) {}

	std::pair<const std::vector<double>, const std::vector<int>> run() {

		while (!m_dijkstraBackend.checkIfAllVerticesHaveBeenProcessed()) {

			VertexData vertexWithMinDistanceToCluster = m_dijkstraBackend.findVertexWithMinimalDistanceToCluster();

			// TODO: pomyslec o tym przypadku
			if (vertexWithMinDistanceToCluster.vertexNumber == -1) {
				break;
			}

			// oznaczamy wezel jako przetworzony
			m_dijkstraBackend.markVertexAsProcessed(vertexWithMinDistanceToCluster.vertexNumber);

			m_dijkstraBackend.performInnerForLoop(vertexWithMinDistanceToCluster, m_graphData);
		}

		return std::make_pair(m_dijkstraBackend.getDistances(), m_dijkstraBackend.getPredecessors());
	}

private:

	DijkstraAlgorithmBackend m_dijkstraBackend;
	std::vector<double> m_graphData;
};

