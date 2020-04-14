#pragma once

#include <utility>
#include <cctype>
#include <algorithm>

class Validator final {

public:

	Validator() = delete;

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
	static std::pair<bool, const char*> validateCommandLineArguments(int argc, char* argv[]) {
		if (argc < 2) {
			return std::make_pair(false, "Too few command line arguments.");
		}

		std::string firstArgument = argv[1];
		bool isFirstArgumentAPositiveNumber = !firstArgument.empty()
			&& std::find_if(firstArgument.begin(), firstArgument.end(),
				[](unsigned char c) { return !std::isdigit(c); }) == firstArgument.end();

		if (!isFirstArgumentAPositiveNumber) {
			return std::make_pair(false, "First command line argument should be a positive number.");
		}

		return std::make_pair(true, "");
	}


	static std::pair<bool, const char*> validateVerticesNumbers(int totalNumberOfVertices, int sourceVertexIndex) {
		if (totalNumberOfVertices <= 0) {
			return std::make_pair(false, "No vertices to process. Exiting.");
		}
		else if (totalNumberOfVertices <= sourceVertexIndex) {
			return std::make_pair(false, "This source vertex is not valid. Exiting.");
		}
		else return std::make_pair(true, "");
	}

};