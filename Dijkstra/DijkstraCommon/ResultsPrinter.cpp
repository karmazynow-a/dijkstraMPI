#include "ResultsPrinter.h"

void ResultsPrinter::printResultingDistances(const std::vector<double>& distances, int sourceVertexIndex) {
	stream << "============ RESULTS ============" << std::endl;
	for (int i = 0; i < distances.size(); ++i) {
		stream << "Distance from vertex " << sourceVertexIndex << " to " << i << ": " << distances.at(i) << std::endl;
	}
}


void ResultsPrinter::printResultingPaths(const std::vector<int>& predecessors, int sourceVertexIndex) {
	stream << "============= PATHS =============" << std::endl;
	for (int i = 0; i < static_cast<int>(predecessors.size()); ++i) {
		std::vector<int> path;
		int currentVertex = i;
		while (currentVertex != -1) {
			path.push_back(currentVertex);
			currentVertex = predecessors.at(currentVertex);
		}
		if (std::find(path.begin(), path.end(), sourceVertexIndex) != path.end()) {
			for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
				stream << path.at(i) << ", ";
			}
		}
		else {
			stream << "Vertex " << i << " unreachable from source vertex.";
		}
		stream << std::endl;
	}
}
