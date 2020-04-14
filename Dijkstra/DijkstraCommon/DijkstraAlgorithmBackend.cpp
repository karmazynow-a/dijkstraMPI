#include "DijkstraAlgorithmBackend.h"

#include <utility>



DijkstraAlgorithmBackend::DijkstraAlgorithmBackend(const std::pair<int, int>& verticesToHandleRange, int totalNumberOfVertices, int sourceVertexIndex) :
	verticesToHandleRange(verticesToHandleRange),
	distances(verticesToHandleRange.second - verticesToHandleRange.first + 1, std::numeric_limits<double>::infinity()),
	predecessors(verticesToHandleRange.second - verticesToHandleRange.first + 1, -1),
	totalNumberOfVertices(totalNumberOfVertices)
{
	if (sourceVertexIndex >= verticesToHandleRange.first && sourceVertexIndex <= verticesToHandleRange.second) {
		distances.at(sourceVertexIndex - verticesToHandleRange.first) = 0;
	}
}


VertexData DijkstraAlgorithmBackend::findVertexWithMinimalDistanceToCluster() {
	int closestVertex = -1;
	double shortestDistance = std::numeric_limits<double>::infinity();
	for (int i = 0; i < distances.size(); ++i) {
		if (processedVerticesCluster.find(i + verticesToHandleRange.first) == processedVerticesCluster.end()) {
			if (distances.at(i) < shortestDistance) {
				closestVertex = verticesToHandleRange.first + i;
				shortestDistance = distances.at(i);
			}
		}
	}
	return (closestVertex != -1 ? VertexData(distances.at(closestVertex - verticesToHandleRange.first), closestVertex) : VertexData());
}


void DijkstraAlgorithmBackend::performInnerForLoop(const VertexData& vertexClosestToCluster, const std::vector<double>& processedPartOfAdjacencyMatrix) {

	int numberOfVerticesToHandle = verticesToHandleRange.second - verticesToHandleRange.first + 1;
	for (int i = 0; i < numberOfVerticesToHandle; ++i) {

		// if vertex has already been processed, we skip it
		if (processedVerticesCluster.find(verticesToHandleRange.first + i) != processedVerticesCluster.end()) {
			continue;
		}

		double valueInMatrix = (processedPartOfAdjacencyMatrix.at(i * totalNumberOfVertices + vertexClosestToCluster.vertexNumber) < 0.001
			? std::numeric_limits<double>::infinity()
			: processedPartOfAdjacencyMatrix.at(i * totalNumberOfVertices + vertexClosestToCluster.vertexNumber));
		double altDistance = valueInMatrix + vertexClosestToCluster.distance;

		if (altDistance < distances.at(i)) {
			distances.at(i) = altDistance;
			predecessors.at(i) = vertexClosestToCluster.vertexNumber;
		}
	}
}