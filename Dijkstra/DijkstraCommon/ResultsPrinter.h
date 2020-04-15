#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

/// <summary>
/// Simple helper class used for printing Dijkstra algorithm 
/// results. Results can be printed to standard output or to 
/// a file. Please note that this printing mechanism is 
/// indeprendent of Log class - if Log is disabled, this class
/// can still print result to output.
/// </summary>
class ResultsPrinter final {

public:

	/// <summary>
	/// Results printer constructor. Sets output stream used for printing. 
	/// </summary>
	/// <param name="stream">
	/// Reference to output stream used by the printer. Please note that
	/// the printer itself stores reference - it is valid as long as the
	/// stream itself it valid. 
	/// </param>
	ResultsPrinter(std::ostream& stream) : stream(stream) {}


	/// <summary>
	/// Prints list of distances from source vertex to output stream. 
	/// </summary>
	/// <param name="distances">
	/// Vector of valid distances from every vertex to source vertex.
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Number of source vertex.
	/// </param>
	void printResultingDistances(const std::vector<double>& distances, int sourceVertexIndex);


	/// <summary>
	/// Prints paths from source vertex to every vertex in graph. If vertex
	/// can not be accessed from source vertex, proper information about this
	/// is printed.
	/// </summary>
	/// <param name="predecessors">
	/// Vector of valid predecessors created using Dijkstra algorithm. 
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Number of source vertex.
	/// </param>
	void printResultingPaths(const std::vector<int>& predecessors, int sourceVertexIndex);


private:

	std::ostream& stream;

};

