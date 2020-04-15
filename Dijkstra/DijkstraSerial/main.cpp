#include "AdjacencyMatrix.h"
#include "Log.h"
#include "ResultsPrinter.h"
#include "CommandLineArgumentsExtractor.h"
#include "Validator.h"
#include "DijkstraSerial.h"

#include <chrono>
#include <fstream>

#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG


int main(int argc, char* argv[]) {

	auto tStart = std::chrono::high_resolution_clock::now();

	// prepare Log class - initialize it with reference to standard output
	Log<SHOULD_LOG> log(std::cout);

	// validate command line arguments
	auto validationResult = Validator::validateCommandLineArguments(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		return 0;
	}

	// read data from command line arguments
	int sourceVertexIndex = CommandLineArgumentsExtractor::extractSourceVertexIndexFromCommandLineArguments(argv);
	std::string filePath = CommandLineArgumentsExtractor::extractInputFileNameFromCommandLineArguments(argc, argv);

	// read graph data from file
	std::unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
	std::vector<double> graphData = matrix->asContinousVector();
	int totalNumberOfVertices = matrix->getNumberOfVertices();
	matrix.reset();

	// perform additional validation
	validationResult = Validator::validateVerticesNumbers(totalNumberOfVertices, sourceVertexIndex);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		return 0;
	}

	auto tBeforeAlgorithmStart = std::chrono::high_resolution_clock::now();

	// run Dijkstra algorithm
	std::pair<int, int> verticesToHandleRange(0, totalNumberOfVertices - 1);
	DijkstraSerial dijkstraAlgorithm(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex, graphData);
	auto dijkstraResults = dijkstraAlgorithm.run();

	auto tBeforePrinting = std::chrono::high_resolution_clock::now();

	// print results to file
	std::ofstream file("resultsSerial.txt");
	ResultsPrinter resultsPrinter(file);
	resultsPrinter.printResultingDistances(dijkstraResults.first, sourceVertexIndex);
	resultsPrinter.printResultingPaths(dijkstraResults.second, sourceVertexIndex);

	auto tEnd = std::chrono::high_resolution_clock::now();

	// log information about elapsed time
	std::chrono::duration<double> diffTotal = tEnd - tStart;
	std::chrono::duration<double> diffSetup = tBeforeAlgorithmStart - tStart;
	std::chrono::duration<double> diffAlgo = tBeforePrinting - tBeforeAlgorithmStart;
	std::chrono::duration<double> diffPrint = tEnd - tBeforePrinting;
	log.logMessage("Total elapsed time: ", diffTotal.count(), "s");
	log.logMessage("Setup took: ", diffSetup.count(), "s");
	log.logMessage("Algorithm took: ", diffAlgo.count(), "s");
	log.logMessage("Printing solution took: ", diffPrint.count(), "s");

	return 0;
}