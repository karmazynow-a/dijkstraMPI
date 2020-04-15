#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

class DijkstraMPISetup final
{
public:
	DijkstraMPISetup() = delete;

	static std::vector<int> divideGraphMatrixIntoChunks(int numberOfVertices, int numberOfProcesses);


	static std::vector<int> computeDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess, int sizeOfSingleDataElement);


	static std::vector<int> computeBufferSizesForEachProcess(const std::vector<int>& numberOfColumnsForEachProcess, int totalNumberOfVertices);


	static std::pair<int, int> computeRangeOfVerticesToHandle(int matrixSide, int numberOfProcesses, int processRank, int numberOfColumnsToHandle);

};

