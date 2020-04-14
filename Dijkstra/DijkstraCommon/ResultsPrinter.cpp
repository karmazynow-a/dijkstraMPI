#include "ResultsPrinter.h"

void ResultsPrinter::printResultingDistances(const std::vector<double>& distances, int sourceVertexIndex) {
	stream << "============ RESULTS ============" << std::endl;
	for (int i = 0; i < distances.size(); ++i) {
		stream << "Distance from vertex " << sourceVertexIndex << " to " << i << ": " << distances.at(i) << std::endl;
	}
}


// TODO: dodac obsluge przypadku jak nie da sie dotrzec 
void ResultsPrinter::printResultingPaths(const std::vector<int>& predecessors) {
	stream << "============ PATHS ============" << std::endl;
	for (int i = 0; i < static_cast<int>(predecessors.size()); ++i) {
		std::vector<int> path;
		int currentVertex = i;
		while (currentVertex != -1) {
			path.push_back(currentVertex);
			currentVertex = predecessors.at(currentVertex);
		}
		for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
			stream << path.at(i) << ", ";
		}
		stream << std::endl;
	}
}
