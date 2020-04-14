// Dijkstra_MPI.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

#include "mpi.h"

#include "AdjacencyMatrix.h"
#include "Log.h"
#include "ResultsPrinter.h"
#include "CommandLineArgumentsHandler.h"
#include "DijkstraAlgorithm.h"
#include "VertexData.h"

#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG


std::vector<int> divideGraphMatrixIntoChunks(int numberOfVertices, int numberOfProcesses) {
	int averageNumberOfColumnsPerProcess = numberOfVertices / numberOfProcesses;
	std::vector<int> numbersOfColumnsForEachProcess(numberOfProcesses, averageNumberOfColumnsPerProcess);
	int numberOfColumnsLeft = numberOfVertices - averageNumberOfColumnsPerProcess * numberOfProcesses;
	for (int i = 0; i < numberOfColumnsLeft; ++i) {
		++numbersOfColumnsForEachProcess.at(i % numbersOfColumnsForEachProcess.size());
	}
	return numbersOfColumnsForEachProcess;
}


std::vector<int> computeDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess, int sizeOfSingleDataElement) {
	std::vector<int> displacements(numbersOfColumnsForEachProcess.size(), 0);
	for (size_t i = 1; i < displacements.size(); ++i) {
		displacements.at(i) = displacements.at(i - 1) + numbersOfColumnsForEachProcess.at(i - 1) * sizeOfSingleDataElement;
	}
	return displacements;
}

std::vector<int> computeNumbersOfVerticesToHandle(int matrixSide, int numberOfProcesses, int processRank, int numberOfColumnsToHandle) {

	int averageNumberOfColumnsPerProcess = matrixSide / numberOfProcesses;
	int numberOfLeftColumns = matrixSide - averageNumberOfColumnsPerProcess * numberOfProcesses;

	int start = processRank * averageNumberOfColumnsPerProcess + (processRank < numberOfLeftColumns ? processRank : numberOfLeftColumns);
	int end = start + numberOfColumnsToHandle - 1;

	std::vector<int> verticesToHandle;
	for (int i = start; i <= end; ++i) {
		verticesToHandle.push_back(i);
	}

	return verticesToHandle;

}






int main(int argc, char* argv[])
{
	int numberOfProcesses = 0;
	int processRank = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	Log<SHOULD_LOG> log(std::cout, processRank);
	
	auto validationResult = CommandLineArgumentsHandler::validateCommandLineArguments(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		MPI_Finalize();
		return 0;
	}

	int sourceVertexIndex = CommandLineArgumentsHandler::extractSourceVertexIndexFromCommandLineArguments(argv);

	std::vector<int> numbersOfColumnsForEachProcess; // ilosc kolumn, jakie obsluzy kazdy z procesow
	std::vector<int> matrixChunksDisplacements;
	std::vector<int> bufferSizes;

	int totalNumberOfVertices = -1;

	std::vector<double> graphData;
	if (processRank == 0) {

		std::string filePath = CommandLineArgumentsHandler::extractInputFileNameFromCommandLineArguments(argc, argv);
		std::unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
		graphData = matrix->asContinousVector();
		totalNumberOfVertices = matrix->getNumberOfVertices();
		log.logMessage("Processing matrix: \n", matrix->toString());

		numbersOfColumnsForEachProcess = divideGraphMatrixIntoChunks(totalNumberOfVertices, numberOfProcesses);
		matrixChunksDisplacements = computeDisplacements(numbersOfColumnsForEachProcess, totalNumberOfVertices);

		std::for_each(numbersOfColumnsForEachProcess.begin(), numbersOfColumnsForEachProcess.end(), [&] (auto& columns) mutable {
			bufferSizes.push_back(columns * totalNumberOfVertices);
		});

	}

	MPI_Bcast(&totalNumberOfVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (totalNumberOfVertices == 0) {
		log.logMessage("No vertices to process. Exiting.");
		MPI_Finalize();
		return 0;
	}

	if (totalNumberOfVertices <= sourceVertexIndex) {
		log.logMessage("This source vertex is not valid. Exiting.");
		MPI_Finalize();
		return 0;
	}

	int numberOfColumnsToHandle = -1;
	MPI_Scatter(numbersOfColumnsForEachProcess.data(), 1, MPI_INT, &numberOfColumnsToHandle, 1, MPI_INT, 0, MPI_COMM_WORLD);

	std::vector<double> matrixChunk (numberOfColumnsToHandle * totalNumberOfVertices, 0.0);

	MPI_Scatterv(graphData.data(), bufferSizes.data(), matrixChunksDisplacements.data(),
		MPI_DOUBLE, matrixChunk.data(), numberOfColumnsToHandle * totalNumberOfVertices, MPI_DOUBLE, 0, MPI_COMM_WORLD);


	std::vector<int> verticesToHandle = computeNumbersOfVerticesToHandle(totalNumberOfVertices, numberOfProcesses, processRank, numberOfColumnsToHandle);

	if (!verticesToHandle.empty()) {
		log.logMessage("This process will handle ", numberOfColumnsToHandle, " vertices in range [", verticesToHandle.front(), ", ", verticesToHandle.back(), "]");
	}
	

	// ALGORITHM
	DijkstraAlgorithm dijkstraAlgorithm(verticesToHandle, totalNumberOfVertices, sourceVertexIndex);

	while (!dijkstraAlgorithm.checkIfAllVerticesHaveBeenProcessed()) {

		// jesli dany wezel nie dostal zadnej kolumny - rozwiazanie TYMCZASOWE, przerobic logike tak zeby nie bylo tego widac
		if (verticesToHandle.size() == 0) {
			VertexData globalMin, localMin;
			MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
			dijkstraAlgorithm.markVertexAsProcessed(globalMin.vertexNumber);
			continue;
		}

		VertexData localMin = dijkstraAlgorithm.findVertexWithMinimalDistanceToCluster();
		VertexData globalMin;

		// W prezentacji - omowic MPI_MINLOC i Allreduce
		MPI_Allreduce(&localMin, &globalMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

		// TODO: pomyslec o tym przypadku
		if (globalMin.vertexNumber == -1) {
			break;
		}

		// oznaczamy wezel jako przetworzony
		dijkstraAlgorithm.markVertexAsProcessed(globalMin.vertexNumber);

		// petla wewnetrzna - tutaj czesc zrownoleglona
		dijkstraAlgorithm.performInnerForLoop(globalMin, matrixChunk);
	}

	std::vector<int> resultsDisplacements;
	if (processRank == 0) {
		resultsDisplacements = computeDisplacements(numbersOfColumnsForEachProcess, 1);
	}


	std::vector<double> globalDistances(totalNumberOfVertices, 0);
	std::vector<int> globalPredecessors(totalNumberOfVertices, -1);

	MPI_Gatherv(dijkstraAlgorithm.getDistances().data(), numberOfColumnsToHandle, MPI_DOUBLE,
		globalDistances.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
		MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Gatherv(dijkstraAlgorithm.getPredecessors().data(), numberOfColumnsToHandle, MPI_INT,
		globalPredecessors.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
		MPI_INT, 0, MPI_COMM_WORLD);


	if (processRank == 0) {
		ResultsPrinter resultsPrinter(std::cout);
		resultsPrinter.printResultingDistances(globalDistances, sourceVertexIndex);
		resultsPrinter.printResultingPaths(globalPredecessors);
	}

	MPI_Finalize();

    return 0;
}

