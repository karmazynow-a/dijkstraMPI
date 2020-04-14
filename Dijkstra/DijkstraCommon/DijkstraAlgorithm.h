#pragma once

#include <set>
#include <vector>
#include "VertexData.h"

class DijkstraAlgorithm {

public:

	DijkstraAlgorithm(const std::vector<int>& verticesToHandle, int totalNumberOfVertices, int sourceVertexIndex) : 
		verticesToHandle(verticesToHandle),
		distances(verticesToHandle.size(), std::numeric_limits<double>::infinity()),
		predecessors(verticesToHandle.size(), -1),
		totalNumberOfVertices(totalNumberOfVertices)
	{
		if (std::find(verticesToHandle.begin(), verticesToHandle.end(), sourceVertexIndex) != verticesToHandle.end()) {
			distances.at(sourceVertexIndex - verticesToHandle.at(0)) = 0;
		}
	}


	bool checkIfAllVerticesHaveBeenProcessed() const {
		return processedVerticesCluster.size() == totalNumberOfVertices;
	}


	VertexData findVertexWithMinimalDistanceToCluster() {
		int minimumIndex = -1;
		double shortestDistance = std::numeric_limits<double>::infinity();

		for (int i = 0; i < distances.size(); ++i) {

			if (processedVerticesCluster.find(i + verticesToHandle.front()) == processedVerticesCluster.end()) {
				if (distances.at(i) < shortestDistance) {
					minimumIndex = verticesToHandle.front() + i;
					shortestDistance = distances.at(i);
				}
			}
		}

		if (minimumIndex != -1) {
			VertexData vertexClosestToCluster {distances.at(minimumIndex - verticesToHandle.front()), minimumIndex };
			return vertexClosestToCluster;
		}
		else return VertexData();
	}


	void markVertexAsProcessed(int vertex) {
		processedVerticesCluster.insert(vertex);
	}


	void performInnerForLoop(const VertexData& vertexClosestToCluster, const std::vector<double>& processedPartOfAdjacencyMatrix) {
		for (int i = 0; i < verticesToHandle.size(); ++i) {

			// jesli wierzcholek zostal juz przetworzony - czyli nalezy do zbioru - pomijamy
			if (processedVerticesCluster.find(verticesToHandle.at(i)) != processedVerticesCluster.end()) {
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

	const std::vector<double>& getDistances() const {
		return distances;
	}

	const std::vector<int>& getPredecessors() const {
		return predecessors;
	}

private:

	std::vector<int> verticesToHandle;
	std::vector<double> distances;
	std::vector<int> predecessors;
	std::set<int> processedVerticesCluster;

	int totalNumberOfVertices;

};