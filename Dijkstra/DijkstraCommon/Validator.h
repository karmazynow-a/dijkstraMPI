/**
*	@file Validator.h
*	@brief This file contains simple class responsible for validation, mainly used with
*	command line arguments.
*/

#pragma once

#include <utility>

/// <summary>
/// Simple helper class used for validation, mainly connected to command 
/// line arguments. All methods in this class are static and the constructor
/// is deleted. All methods return pair of boolean and C-style string with
/// validation result.
/// </summary>
class Validator final {

public:

	/// <summary>
	/// Deleted constructor - all methods in this class are static, there is
	/// no need for creating instance of it.
	/// </summary>
	Validator() = delete;


	/// <summary>
	/// Simple command line arguments validator. User should provide at least
	/// one argument - positive number indicating source vertex. 
	/// </summary>
	/// <returns>
	/// Pair with boolean and C-style const string. Boolean indicates if 
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
	/// Simple vertices data validator. It checks if there are any vertices to process
	/// by Dijkstra algorithm and if source vertex can be found in input graph (if source
	/// vertex number is not bigger than or equal to total number of vertices in graph).
	/// </summary>
	/// <returns>
	/// Pair with boolean and C-style const string. Boolean indicates if 
	/// command line arguments are valid. String contains error message. If
	/// arguments are valid, this string is empty.
	/// </returns>
	/// <param name="totalNumberOfVertices">
	/// Total number of vertices in processed graph.
	/// </param>
	/// <param name="sourceVertexIndex">
	/// Index of source vertex provided by user.
	/// </param>
	static std::pair<bool, const char*> validateVerticesNumbers(int totalNumberOfVertices, int sourceVertexIndex);

};