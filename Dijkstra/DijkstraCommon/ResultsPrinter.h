#pragma once

#include <iostream>
#include <vector>

/// <summary>
/// Simple helper class used for printing Dijkstra algorithm 
/// results. Results can be printed to cout or file. 
/// </summary>
class ResultsPrinter final {

public:

	ResultsPrinter(std::ostream& stream) : stream(stream) {}


	void printResultingDistances(const std::vector<double>& distances, int sourceVertexIndex);


	// TODO: dodac obsluge przypadku jak nie da sie dotrzec 
	void printResultingPaths(const std::vector<int>& predecessors);


private:

	std::ostream& stream;

};

