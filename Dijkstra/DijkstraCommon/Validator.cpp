#include "Validator.h"

#include <cctype>
#include <algorithm>
#include <string>

std::pair<bool, const char*> Validator::validateCommandLineArguments(int argc, char* argv[]) {
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


std::pair<bool, const char*> Validator::validateVerticesNumbers(int totalNumberOfVertices, int sourceVertexIndex) {
	if (totalNumberOfVertices <= 0) {
		return std::make_pair(false, "No vertices to process. Exiting.");
	}
	else if (totalNumberOfVertices <= sourceVertexIndex) {
		return std::make_pair(false, "This source vertex is not valid. Exiting.");
	}
	else return std::make_pair(true, "");
}