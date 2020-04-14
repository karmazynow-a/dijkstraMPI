#pragma once

#include <limits>

struct VertexData {

	VertexData(double distance = std::numeric_limits<double>::infinity(), int number = -1) 
		: distance(distance), vertexNumber(number) {}

	double distance;
	int vertexNumber;

};