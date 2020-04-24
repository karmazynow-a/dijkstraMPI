#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>

#include <mpi.h>

#include "AdjacencyMatrix.h"
#include "Log.h"
#include "ResultsPrinter.h"
#include "CommandLineArgumentsExtractor.h"
#include "DijkstraMPISetup.h"
#include "DijkstraMPI.h"
#include "Validator.h"

#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG


int main(int argc, char* argv[])
{
	auto tStart = std::chrono::high_resolution_clock::now();

	// initialize MPI
	int numberOfProcesses = 0;
	int processRank = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	// prepare Log class - initialize it with reference to standard output
	Log<SHOULD_LOG> log(std::cout, processRank);
	
	// validate command line arguments
	auto validationResult = Validator::validateCommandLineArguments(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		MPI_Finalize();
		return 0;
	}

	// prepare structures used to transfer data from process 0 using MPI
	std::vector<int> numbersOfColumnsForEachProcess;
	std::vector<int> matrixChunksDisplacements;
	std::vector<int> bufferSizes;
	std::vector<double> graphData;
	int totalNumberOfVertices = -1;

	// read data from command line arguments
	int sourceVertexIndex = CommandLineArgumentsExtractor::extractSourceVertexIndexFromCommandLineArguments(argv);

	if (processRank == 0) {

		// read data from command line arguments
		std::string filePath = CommandLineArgumentsExtractor::extractInputFileNameFromCommandLineArguments(argc, argv);

		// read graph data from file
		std::unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
		graphData = matrix->asContinousVector();
		totalNumberOfVertices = matrix->getNumberOfVertices();

		// prepare structures used to transfer data from process 0 using MPI
		numbersOfColumnsForEachProcess = DijkstraMPISetup::divideGraphMatrixIntoChunks(totalNumberOfVertices, numberOfProcesses);
		matrixChunksDisplacements = DijkstraMPISetup::computeDisplacements(numbersOfColumnsForEachProcess, totalNumberOfVertices);
		bufferSizes = DijkstraMPISetup::computeBufferSizesForEachProcess(numbersOfColumnsForEachProcess, totalNumberOfVertices);

	}

	// broadcast graph size using MPI
	MPI_Bcast(&totalNumberOfVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// perform additional validation
	validationResult = Validator::validateVerticesNumbers(totalNumberOfVertices, sourceVertexIndex);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		MPI_Finalize();
		return 0;
	}

	// send graph data to all processes
	int numberOfColumnsToHandle = -1;
	MPI_Scatter(numbersOfColumnsForEachProcess.data(), 1, MPI_INT, &numberOfColumnsToHandle, 1, MPI_INT, 0, MPI_COMM_WORLD);

	std::vector<double> matrixChunk (numberOfColumnsToHandle * totalNumberOfVertices, 0.0);
	MPI_Scatterv(graphData.data(), bufferSizes.data(), matrixChunksDisplacements.data(),
		MPI_DOUBLE, matrixChunk.data(), numberOfColumnsToHandle * totalNumberOfVertices, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	graphData.clear();

	// create new communicator - to distinguish idle processes from those that calculate algorithm
	MPI_Comm activeProcessesCommunicator;
	MPI_Comm_split(MPI_COMM_WORLD, numberOfColumnsToHandle > 0, processRank, &activeProcessesCommunicator);

	if (numberOfColumnsToHandle > 0) {

		std::pair<int, int> verticesToHandleRange = DijkstraMPISetup::computeRangeOfVerticesToHandle(totalNumberOfVertices, numberOfProcesses, processRank, numberOfColumnsToHandle);
		log.logMessage("This process will handle ", numberOfColumnsToHandle, " vertices in range [", verticesToHandleRange.first, ", ", verticesToHandleRange.second, "]");

		auto tBeforeAlgorithmStart = std::chrono::high_resolution_clock::now();

		// run Dijkstra algorithm
		DijkstraMPI dijkstraAlgorith(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex, matrixChunk);
		auto dijkstraResult = dijkstraAlgorith.run(activeProcessesCommunicator);

		std::vector<int> resultsDisplacements;
		if (processRank == 0) {
			numbersOfColumnsForEachProcess.erase(std::remove(numbersOfColumnsForEachProcess.begin(), numbersOfColumnsForEachProcess.end(), 0), numbersOfColumnsForEachProcess.end());
			resultsDisplacements = DijkstraMPISetup::computeDisplacements(numbersOfColumnsForEachProcess, 1);
		}

		// prepare structures for gathering results
		std::vector<double> globalDistances(totalNumberOfVertices, 0);
		std::vector<int> globalPredecessors(totalNumberOfVertices, -1);

		// gather results
		MPI_Gatherv(dijkstraResult.first.data(), numberOfColumnsToHandle, MPI_DOUBLE,
			globalDistances.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
			MPI_DOUBLE, 0, activeProcessesCommunicator);

		MPI_Gatherv(dijkstraResult.second.data(), numberOfColumnsToHandle, MPI_INT,
			globalPredecessors.data(), numbersOfColumnsForEachProcess.data(), resultsDisplacements.data(),
			MPI_INT, 0, activeProcessesCommunicator);

		auto tBeforePrinting = std::chrono::high_resolution_clock::now();

		if (processRank == 0) {

			auto tBeforePrinting = std::chrono::high_resolution_clock::now();

			// print results to file
			std::ofstream file("resultsMPI.txt");
			ResultsPrinter resultsPrinter(file);
			resultsPrinter.printResultingDistances(globalDistances, sourceVertexIndex);
			resultsPrinter.printResultingPaths(globalPredecessors, sourceVertexIndex);

			auto tEnd = std::chrono::high_resolution_clock::now();

			// log information about elapsed time
			std::chrono::duration<double> diff = tEnd - tStart;
			std::chrono::duration<double> diffTotal = tEnd - tStart;
			std::chrono::duration<double> diffSetup = tBeforeAlgorithmStart - tStart;
			std::chrono::duration<double> diffAlgo = tBeforePrinting - tBeforeAlgorithmStart;
			std::chrono::duration<double> diffPrint = tEnd - tBeforePrinting;
			log.logMessage("Total elapsed time: ", diffTotal.count(), "s");
			log.logMessage("Setup took: ", diffSetup.count(), "s");
			log.logMessage("Algorithm took: ", diffAlgo.count(), "s");
			log.logMessage("Printing solution took: ", diffPrint.count(), "s");
		}

	}

	// finish using MPI
	MPI_Comm_free(&activeProcessesCommunicator);
	MPI_Finalize();

    return 0;
}
