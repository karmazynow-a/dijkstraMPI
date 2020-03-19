// Dijkstra_MPI.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

#include "mpi.h"

#include "AdjacencyMatrix.h"



std::vector<int> divideGraphMatrixIntoChunks(const AdjacencyMatrix& graph, int numberOfProcesses) {
	int averageNumberOfColumnsPerProcess = graph.side / numberOfProcesses;
	std::vector<int> numbersOfColumnsForEachProcess(numberOfProcesses, averageNumberOfColumnsPerProcess);
	int numberOfColumnsLeft = graph.side - averageNumberOfColumnsPerProcess * numberOfProcesses;
	for (int i = 0; i < numberOfColumnsLeft; ++i) {
		++numbersOfColumnsForEachProcess.at(i % numbersOfColumnsForEachProcess.size());
	}
	return numbersOfColumnsForEachProcess;
}

void printMatrixColumnDistribution(const std::vector<int>& numbersOfColumnsForEachProcess) {
	std::for_each(numbersOfColumnsForEachProcess.begin(), numbersOfColumnsForEachProcess.end(), [idx = 0](auto& numberOfColumns) mutable {
		std::cout << "Process " << ++idx << " will receive " << numberOfColumns << " columns." << std::endl;
	});
}

std::vector<int> computeElementDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess, int sizeOfSingleColumn) {
	std::vector<int> displacements(numbersOfColumnsForEachProcess.size(), 0);
	for (size_t i = 1; i < displacements.size(); ++i) {
		displacements.at(i) = displacements.at(i - 1) + numbersOfColumnsForEachProcess.at(i - 1) * sizeOfSingleColumn;
	}
	return displacements;
}


std::vector<int> computeResultsDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess) {
	std::vector<int> displacements(numbersOfColumnsForEachProcess.size(), 0);
	for (size_t i = 1; i < displacements.size(); ++i) {
		displacements.at(i) = displacements.at(i - 1) + numbersOfColumnsForEachProcess.at(i - 1);
	}
	return displacements;
}

std::vector<int> computeNumbersOfVerticesToHandle(int matrixSide, int numberOfProcesses, int processRank, int numberOfColumnsToHandle) {

	int averageNumberOfColumnsPerProcess = matrixSide / numberOfProcesses;
	int numberOfLeftColumns = matrixSide - averageNumberOfColumnsPerProcess * numberOfProcesses;

	int start = processRank * averageNumberOfColumnsPerProcess + (processRank < numberOfLeftColumns ? processRank : numberOfLeftColumns);
	int end = start + numberOfColumnsToHandle - 1;

	std::cout << "Process " << processRank << ": (" << start << ", " << end << ") " << std::endl;

	std::vector<int> verticesToHandle;
	for (int i = start; i <= end; ++i) {
		verticesToHandle.push_back(i);
	}

	return verticesToHandle;

}


int findVertexWithMinimalDistanceToCluster(const std::vector<double>& distancesLocal, const std::vector<int>& cluster, int vertexRangeStart) {
	int localMinimumIndex = -1;
	double shortestDistance = std::numeric_limits<double>::infinity();

	for (int i = 0; i < distancesLocal.size(); ++i) {
		if (std::find(cluster.begin(), cluster.end(), vertexRangeStart + i) == cluster.end()) {
			if (distancesLocal.at(i) < shortestDistance) {
				localMinimumIndex = vertexRangeStart + i;
				shortestDistance = distancesLocal.at(i);
			}
		}
	}
	return localMinimumIndex;
}


int main(int argc, char* argv[])
{

	int numberOfProcesses = 0;
	int processRank = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	int sourceVertexIndex = -1;

	std::vector<int> numbersOfColumnsForEachProcess(numberOfProcesses, 0); // ilosc kolumn, jakie obsluzy kazdy z procesow
	std::vector<int> displacements;
	std::vector<int> bufferSizes;

	int matrixSide = -1;


	std::vector<double> graphData;
	if (processRank == 0) {
		std::cout << "Please enter source vertex index: ";
		std::cin >> sourceVertexIndex;

		AdjacencyMatrix graph;
		for (int i = 0; i < graph.side; ++i) {
			for (int j = 0; j < graph.side; ++j) {
				graphData.push_back(graph.matrix[i][j]);
			}
		}
		matrixSide = graph.side;

		numbersOfColumnsForEachProcess = divideGraphMatrixIntoChunks(graph, numberOfProcesses);
		printMatrixColumnDistribution(numbersOfColumnsForEachProcess);
		displacements = computeElementDisplacements(numbersOfColumnsForEachProcess, graph.side);

		std::for_each(numbersOfColumnsForEachProcess.begin(), numbersOfColumnsForEachProcess.end(), [&] (auto& columns) mutable {
			bufferSizes.push_back(columns * matrixSide);
		});

	}



	

	int numberOfColumnsToHandle = -1;
	MPI_Scatter(numbersOfColumnsForEachProcess.data(), 1, MPI_INT, &numberOfColumnsToHandle, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&matrixSide, 1, MPI_INT, 0, MPI_COMM_WORLD);

	std::vector<double> matrixChunk (numberOfColumnsToHandle * matrixSide, 0.0);

	MPI_Scatterv(graphData.data(), bufferSizes.data(), displacements.data(),
		MPI_DOUBLE, matrixChunk.data(), numberOfColumnsToHandle * matrixSide, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	std::vector<std::vector<double>> matrixChunk2D(numberOfColumnsToHandle, std::vector<double>(matrixSide, 0));
	for (int i = 0; i < numberOfColumnsToHandle; ++i) {
		for (int j = 0; j < matrixSide; ++j) {
			matrixChunk2D[i][j] = matrixChunk.at(i * matrixSide + j);
		}
	}

	std::vector<int> verticesToHandle = computeNumbersOfVerticesToHandle(matrixSide, numberOfProcesses, processRank, numberOfColumnsToHandle);

	// ALGORITHM
	std::vector<double> distances (numberOfColumnsToHandle, std::numeric_limits<double>::infinity());
	std::vector<int> predecessors(numberOfColumnsToHandle, -1);

	MPI_Bcast(&sourceVertexIndex, 1, MPI_INT, 0, MPI_COMM_WORLD);

	std::vector<int> processedVertexCluster{};

	// ustawiamy odleglosc od wezla startowego na 0 dla jego samego w odpowiedzialnym
	// za niego wezle
	if (std::find(verticesToHandle.begin(), verticesToHandle.end(), sourceVertexIndex) != verticesToHandle.end()) {
		std::cout << processRank << " has source vertex" << std::endl;
		distances.at(sourceVertexIndex - verticesToHandle.at(0)) = 0;
	}

	struct vertexData {
		double distance = std::numeric_limits<double>::infinity();
		int vertexNumber = -1;
	};

	std::cout << "Process with id=" << processRank << " knows that it will have to handle: " << numberOfColumnsToHandle << " columns." << std::endl;

	while (processedVertexCluster.size() < matrixSide) {

		// jesli dany wezel nie dostal zadnej kolumny - rozwiazanie TYMCZASOWE, przerobic logike tak zeby nie bylo tego widac
		if (verticesToHandle.size() == 0) {
			vertexData globalMin;
			vertexData localMin;
			MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
			processedVertexCluster.push_back(globalMin.vertexNumber);
			continue;
		}

		int u = findVertexWithMinimalDistanceToCluster(distances, processedVertexCluster, verticesToHandle.at(0));
		vertexData localMin;
		if (u != -1) {
			localMin.distance = distances[u - verticesToHandle.at(0)];
			localMin.vertexNumber = u;
		}
		
		vertexData globalMin;

		// W prezentacji - omowic MPI_MINLOC i Allreduce
		MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

		// TODO: pomyslec o tym przypadku
		if (globalMin.vertexNumber == -1) {
			break;
		}

		// oznaczamy wezel jako przetworzony
		processedVertexCluster.push_back(globalMin.vertexNumber);

		for (int i = 0; i < verticesToHandle.size(); ++i) {

			// jesli wierzcholek zostal juz przetworzony - czyli nalezy do zbioru - pomijamy
			if (std::find(processedVertexCluster.begin(), processedVertexCluster.end(), verticesToHandle.at(i)) != processedVertexCluster.end()) {
				continue;
			}

			double valueInMatrix = (matrixChunk2D[i][globalMin.vertexNumber] < 0.001 ? std::numeric_limits<double>::infinity() : matrixChunk2D[i][globalMin.vertexNumber]);
			double altDistance = valueInMatrix + globalMin.distance;

			if (altDistance < distances.at(i)) {
				distances.at(i) = altDistance;
				predecessors.at(i) = globalMin.vertexNumber;
			}
		}

	}

	std::vector<int> resultsDisplacements;
	if (processRank == 0) {
		resultsDisplacements = computeResultsDisplacements(numbersOfColumnsForEachProcess);
	}


	std::vector<double> globalDistances(matrixSide, 0);
	std::vector<int> globalPredecessors(matrixSide, -1);

	MPI_Gatherv(distances.data(), distances.size(), MPI_DOUBLE,
		globalDistances.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
		MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Gatherv(predecessors.data(), predecessors.size(), MPI_INT,
		globalPredecessors.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
		MPI_INT, 0, MPI_COMM_WORLD);


	if (processRank == 0) {
		std::cout << "============ RESULTS ============" << std::endl;
		for (int i = 0; i < globalDistances.size(); ++i) {
			std::cout << "Distance from vertex: " << i << ": " << globalDistances.at(i) << std::endl;
		}
	}

	// TODO: dodac obsluge przypadku jak nie da sie dotrzec 
	if (processRank == 0) {
		std::cout << "============ PATHS ============" << std::endl;
		for (int i = 0; i < matrixSide; ++i) {
			std::vector<int> path;
			int currentVertex = i;
			while (currentVertex != -1) {
				path.push_back(currentVertex);
				currentVertex = globalPredecessors.at(currentVertex);
			}
			for (int i = path.size() - 1; i >= 0; --i) {
				std::cout << path.at(i) << ", ";
			}
			std::cout << std::endl;
		}
	}

	MPI_Finalize();

    return 0;
}

