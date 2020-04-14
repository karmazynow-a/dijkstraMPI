#include "AdjacencyMatrix.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>

std::string AdjacencyMatrix::toString() const {
	std::stringstream stringRepresentation;
	for (int i = 0; i < m_numberOfVertices; ++i) {
		for (int j = 0; j < m_numberOfVertices; ++j) {
			stringRepresentation << std::setw(5) << m_matrix.at(j * m_numberOfVertices + i) << " ";
		}
		stringRepresentation << std::endl;
	}
	return stringRepresentation.str();
}


void AdjacencyMatrix::readDataFromFile(const std::string& filePath) {

	std::ifstream infile(filePath, std::ifstream::in);
	if (!infile.is_open()) {
		std::cout << "Plik " << filePath << " nie istnieje." << std::endl;
		return;
	}

	infile >> m_numberOfVertices;
	m_matrix.resize(m_numberOfVertices * m_numberOfVertices, 0);
	for (int i = 0; i < m_numberOfVertices; ++i) {
		for (int j = 0; j < m_numberOfVertices; ++j) {
			infile >> m_matrix.at(j * m_numberOfVertices + i);
		}
	}
}
