#include "AdjacencyMatrix.h"
#include "Log.h"
#include "ResultsPrinter.h"
#include "CommandLineArgumentsHandler.h"
#include "DijkstraAlgorithm.h"
#include "VertexData.h"

#include <set>
#include <numeric>

#ifndef SHOULD_LOG
#define SHOULD_LOG true
#endif // !SHOULD_LOG


int main(int argc, char* argv[]) {

	Log<SHOULD_LOG> log(std::cout);

	auto validationResult = CommandLineArgumentsHandler::validateCommandLineArguments(argc, argv);
	if (!validationResult.first) {
		log.logMessage(validationResult.second);
		return 0;
	}
	int sourceVertexIndex = CommandLineArgumentsHandler::extractSourceVertexIndexFromCommandLineArguments(argv);
	std::string filePath = CommandLineArgumentsHandler::extractInputFileNameFromCommandLineArguments(argc, argv);


	std::unique_ptr<AdjacencyMatrix> matrix = AdjacencyMatrix::fromFile(filePath);
	std::vector<double> graphData = matrix->asContinousVector();
	int totalNumberOfVertices = matrix->getNumberOfVertices();

	if (totalNumberOfVertices == 0) {
		log.logMessage("No vertices to process. Exiting.");
		return 0;
	}

	if (totalNumberOfVertices <= sourceVertexIndex) {
		log.logMessage("This source vertex is not valid. Exiting.");
		return 0;
	}

	log.logMessage("Processing matrix: \n", matrix->toString());

	std::vector<int> verticesToHandle(totalNumberOfVertices);
	std::iota(verticesToHandle.begin(), verticesToHandle.end(), 0);
	DijkstraAlgorithm dijkstraAlgorithm(verticesToHandle, totalNumberOfVertices, sourceVertexIndex);

	while (!dijkstraAlgorithm.checkIfAllVerticesHaveBeenProcessed()) {

		VertexData vertexWithMinDistanceToCluster = dijkstraAlgorithm.findVertexWithMinimalDistanceToCluster();

		// TODO: pomyslec o tym przypadku
		if (vertexWithMinDistanceToCluster.vertexNumber == -1) {
			break;
		}

		// oznaczamy wezel jako przetworzony
		dijkstraAlgorithm.markVertexAsProcessed(vertexWithMinDistanceToCluster.vertexNumber);

		dijkstraAlgorithm.performInnerForLoop(vertexWithMinDistanceToCluster, graphData);

	}

	ResultsPrinter resultsPrinter(std::cout);
	resultsPrinter.printResultingDistances(dijkstraAlgorithm.getDistances(), sourceVertexIndex);
	resultsPrinter.printResultingPaths(dijkstraAlgorithm.getPredecessors());

	return 0;
}