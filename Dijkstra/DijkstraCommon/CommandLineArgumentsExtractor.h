#pragma once

#include <string>

/// <summary>
/// Simple helper class that contains static methods related to
/// extracting command line arguments. It is designed in such a way
/// that command line arguments should be validated before using it.
/// </summary>
class CommandLineArgumentsExtractor final {

public:

	/// <summary>
	/// Since this class contains only static methods, its constructor
	/// has been deleted.
	/// </summary>
	CommandLineArgumentsExtractor() = delete;


	/// <summary>
	/// Simple method that extracts source vertex index from command line arguments.
	/// Souce vertex should be first command line argument given by user when running
	/// program. This method assumes that command line arguments have been validated.
	/// </summary>
	/// <returns>
	/// Integer representing index of source vertex.
	/// </returns>
	/// <param name="argv">
	/// C-style array with command line arguments. This method is designed in such
	/// a way that argv should be passed to it directly from main function arguments.
	/// </param>
	static inline int extractSourceVertexIndexFromCommandLineArguments(char* argv[]) {
		return std::stoi(argv[1]);
	}


	/// <summary>
	/// Simple method that extracts data file name from command line arguments. This
	/// name should be second command line argument given by user. If it has not been
	/// provided, default value is returned. This method assumes that command line 
	/// arguments have been validated.
	/// </summary>
	/// <returns>
	/// Data file name in form of a C++-style string.
	/// </returns>
	/// <param name="argc">
	/// Number of command line arguments.
	/// </param>
	/// <param name="argv">
	/// C-style array with command line arguments. This method is designed in such
	/// a way that argv should be passed to it directly from main function arguments.
	/// </param>
	static inline std::string extractInputFileNameFromCommandLineArguments(int argc, char* argv[]) {
		return argc > 2 ? argv[2] : "../data/graph.dat";
	}
};


