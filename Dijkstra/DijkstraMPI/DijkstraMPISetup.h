/**
*	@file DijkstraMPISetup.h
*	@brief This file contains DijkstraMPISetup class resposible for preparing
*	data related to dividing adjacency matrix into parts (each process handles
*	one part).
*/


#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

/// <summary>
/// This class contains static methods used by root process to divide adjacency
/// matrix into chunks. Each chunk is handled by one process. Please note that
/// all methods are static.
/// </summary>
class DijkstraMPISetup final
{
public:

	/// <summary>
	/// Default constructor. Because this class contains only static methods, it
	/// has been deleted.
	/// </summary>
	DijkstraMPISetup() = delete;

	/// <summary>
	/// Static method responsible for dividing adjacency matrix into chunks that
	/// will be handled by separate processes. By dividing into chunks we mean 
	/// computing number of columns each process will handle.
	/// </summary>
	/// <returns>
	/// Vector that contains number of adjacency matrix columns each process will
	/// handle (in other words, number of vertices each process will handle).
	/// </returns>
	/// <param name="numberOfVertices">
	/// Total number of vertices in the graph.
	/// </param>
	/// <param name="numberOfProcesses">
	/// Total number of processes that take part in algorithm execution.
	/// </param>
	static std::vector<int> divideGraphMatrixIntoChunks(int numberOfVertices, int numberOfProcesses);


	/// <summary>
	/// Static method responsible for computing data displacements for each process.
	/// Used only by root process to distribute and gather data using MPI_Scatterv and
	/// MPI_Gatherv.
	/// </summary>
	/// <returns>
	/// Vector of displacements that are arranged using ranks given by MPI.
	/// </returns>
	/// <param name="numberOfColumnsForEachProcess">
	/// Numbers of adjacency matrix columns (graph vertices) each process should 
	/// handle. Should be arranged using ranks given by MPI.
	/// </param>
	/// <param name="sizeOfSingleDataElement">
	/// Size of single data element - this should be understood as number of entries
	/// in array. For example is single data element is a matrix column, it should be
	/// set to number of entries in a column. If single data element is scalar (like 
	/// double), it should be set to one.
	/// </param>
	static std::vector<int> computeDisplacements(const std::vector<int>& numbersOfColumnsForEachProcess, int sizeOfSingleDataElement);


	/// <summary>
	/// Static method responsible for computing buffer sizes for each process. Used
	/// only by root process to distribute data using MPI_Scatterv. 
	/// </summary>
	/// <returns>
	/// Vector of buffer sizes. Sizes are arranged using ranks given by MPI.
	/// </returns>
	/// <param name="numberOfColumnsForEachProcess">
	/// Numbers of adjacency matrix columns (graph vertices) each process should 
	/// handle. Should be arranged using ranks given by MPI.
	/// </param>
	/// <param name="totalNumberOfVertices">
	/// Total number of vertices in processed graph.
	/// </param>
	static std::vector<int> computeBufferSizesForEachProcess(const std::vector<int>& numberOfColumnsForEachProcess, int totalNumberOfVertices);


	/// <summary>
	/// Static method responsible for computing range of vertices that process
	/// with given id will handle. 
	/// </summary>
	/// <returns>
	/// Pair of integers that represents range of vertices given process will 
	/// handle. This range/interval can be written as [a, b] (it is closed).
	/// </returns>
	/// <param name="matrixSide">
	/// Size of adjacency matrix (in other words, number of all vertices in 
	/// processed graph). 
	/// </param>
	/// <param name="numberOfProcesses">
	/// Total number of processes that take part in algorithm execution.
	/// </param>
	/// <param name="processRank">
	/// Current process rank (value from MPI_Comm_rank function).
	/// </param>
	/// <param name="numberOfColumnsToHandle">
	/// Number of adjacency matrix columns (vertices) current process should handle.
	/// </param>
	static std::pair<int, int> computeRangeOfVerticesToHandle(int matrixSide, int numberOfProcesses, int processRank, int numberOfColumnsToHandle);

};

