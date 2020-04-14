#include "CommandLineArgumentsHandler.h"

#include <algorithm>
#include <cctype>

std::pair<bool, const char*> CommandLineArgumentsHandler::validateCommandLineArguments(int argc, char* argv[]) {
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


int CommandLineArgumentsHandler::extractSourceVertexIndexFromCommandLineArguments(char* argv[]) {
	const std::string& firstCommandLineArgument = argv[1];
	return std::stoi(firstCommandLineArgument);
}


std::string CommandLineArgumentsHandler::extractInputFileNameFromCommandLineArguments(int argc, char* argv[]) {
	return argc > 2 ? argv[2] : "../data/graph.dat";
}