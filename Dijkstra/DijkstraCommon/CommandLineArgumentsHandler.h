#pragma once

#include <string>

/// <summary>
/// Simple helper class that contains static methods related to
/// handling command line arguments. It is designed in such a way
/// that validateCommandLineArguments() method should always be 
/// called first.
/// </summary>
class CommandLineArgumentsHandler final {

public:

	/// <summary>
	/// Since this class contains only static methods, its constructor
	/// has been deleted.
	/// </summary>
	CommandLineArgumentsHandler() = delete;


	/// <summary>
	/// Simple command line arguments validator. User should provide at least
	/// one argument - positive number indicating source vertex. 
	/// </summary>
	/// <returns>
	/// Pair with boolean and c-style const string. Boolean indicates if 
	/// command line arguments are valid. String contains error message. If
	/// arguments are valid, this string is empty.
	/// </returns>
	/// <param name="argc">
	/// Number of command line arguments.
	/// </param>
	/// <param name="argv">
	/// C-style array with command line arguments. This method is designed in such
	/// a way that argv should be passed to it directly from main function arguments.
	/// </param>
	static std::pair<bool, const char*> validateCommandLineArguments(int argc, char* argv[]);


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
	static int extractSourceVertexIndexFromCommandLineArguments(char* argv[]);


	/// <summary>
	/// Simple method that extracts data file name from command line arguments. This
	/// name should be second command line argument given by user. If it has not been
	/// provided, default value is returned. 
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
	static std::string extractInputFileNameFromCommandLineArguments(int argc, char* argv[]);
};


