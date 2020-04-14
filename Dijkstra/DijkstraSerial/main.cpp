#include "AdjacencyMatrix.h"
#include "Log.h"
#include "ResultsPrinter.h"
#include "CommandLineArgumentsExtractor.h"
#include "Validator.h"
#include "DijkstraSerial.h"

#include <chrono>
#include <numeric>
#include <fstream>

#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG


int main(int argc, char* argv[]) {

	auto tStart = std::chrono::high_resolution_clock::now();

	Log<SHOULD_LOG> log(std::cout);

	auto validationResult = Validator::validateCommandLineArguments(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		return 0;
	}
	int sourceVertexIndex = CommandLineArgumentsExtractor::extractSourceVertexIndexFromCommandLineArguments(argv);
	std::string filePath = CommandLineArgumentsExtractor::extractInputFileNameFromCommandLineArguments(argc, argv);

	std::unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
	std::vector<double> graphData = matrix->asContinousVector();
	int totalNumberOfVertices = matrix->getNumberOfVertices();

	validationResult = Validator::validateVerticesNumbers(totalNumberOfVertices, sourceVertexIndex);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		return 0;
	}

	auto tBeforeAlgorithmStart = std::chrono::high_resolution_clock::now();

	std::pair<int, int> verticesToHandleRange(0, totalNumberOfVertices - 1);
	DijkstraSerial dijkstraAlgorithm(verticesToHandleRange, totalNumberOfVertices, sourceVertexIndex, graphData);
	auto dijkstraResults = dijkstraAlgorithm.run();

	auto tBeforePrinting = std::chrono::high_resolution_clock::now();
	std::ofstream file("res.txt");
	ResultsPrinter resultsPrinter(file);
	resultsPrinter.printResultingDistances(dijkstraResults.first, sourceVertexIndex);
	resultsPrinter.printResultingPaths(dijkstraResults.second, sourceVertexIndex);

	auto tEnd = std::chrono::high_resolution_clock::now();
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