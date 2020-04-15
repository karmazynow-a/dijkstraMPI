#pragma once

#include <limits>

/// <summary>
/// Simple helper struct that contains current data about distance 
/// from source vertex as well as number of vertex. Please do not
/// create instances of this struct on your own - use function 
/// createVertexDataInstance instead - it initializes struct properties
/// to default values.
/// </summary>
struct VertexData {

	double distance;
	int vertexNumber;

};


/// <summary>
/// Creates instance of VertexData structure. Takes two input arguments,
/// if any of them is not given, uses default values.
/// </summary>
/// <returns>
/// Created instance of VertexData structure.
/// </returns>
/// <param name="distance">
/// Distance from source vertex to this vertex. If not given, this 
/// function sets it to default value of infinity.
/// </param>
/// <param name="number">
/// Number of vertex. If not given, this functions sets it to default
/// value of -1.
/// </param>
inline VertexData createVertexDataInstance(double distance = std::numeric_limits<double>::infinity(), int number = -1) {
	return VertexData { distance, number };
}