#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

class DijkstraMPISetup final
{
public:
	DijkstraMPISetup() = delete;

	static std::vector<int> divideGraphMatrixIntoChunks(int numberOfVertices, int numberOfProcesses) {
		int averageNumberOfColumnsPerProcess = numberOfVertices / numberOfProcesses;
		std::vector<int> numbersOfColumnsForEachProcess(numberOfProcesses, averageNumberOfColumnsPerProcess);
		int numberOfColumnsLeft = numberOfVertices - averageNumberOfColumnsPerProcess * numberOfProcesses;
		for (int i = 0; i < numberOfColumnsLeft; ++i) {
			++numbersOfColumnsForEachProcess.at(i % numbersOfColumnsForEachProcess.size());
		}
		return numbersOfColumnsForEachProcess;
	}


	static std::vector<int> computeDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess, int sizeOfSingleDataElement) {
		std::vector<int> displacements(numbersOfColumnsForEachProcess.size(), 0);
		for (size_t i = 1; i < displacements.size(); ++i) {
			displacements.at(i) = displacements.at(i - 1) + numbersOfColumnsForEachProcess.at(i - 1) * sizeOfSingleDataElement;
		}
		return displacements;
	}

	static std::vector<int> computeBufferSizesForEachProcess(const std::vector<int>& numberOfColumnsForEachProcess, int totalNumberOfVertices) {
		std::vector<int> bufferSizesForEachProcess;
		std::for_each(numberOfColumnsForEachProcess.begin(), numberOfColumnsForEachProcess.end(), [&](auto& columns) mutable {
			bufferSizesForEachProcess.push_back(columns * totalNumberOfVertices);
		});
		return bufferSizesForEachProcess;
	}


	static std::pair<int, int> computeRangeOfVerticesToHandle(int matrixSide, int numberOfProcesses, int processRank, int numberOfColumnsToHandle) {
		int averageNumberOfColumnsPerProcess = matrixSide / numberOfProcesses;
		int numberOfLeftColumns = matrixSide - averageNumberOfColumnsPerProcess * numberOfProcesses;
		int firstVertexIndex = processRank * averageNumberOfColumnsPerProcess + (processRank < numberOfLeftColumns ? processRank : numberOfLeftColumns);
		return std::make_pair(firstVertexIndex, firstVertexIndex + numberOfColumnsToHandle - 1);
	}



};

