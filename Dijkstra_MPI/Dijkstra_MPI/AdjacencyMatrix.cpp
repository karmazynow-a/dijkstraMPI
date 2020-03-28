
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "AdjacencyMatrix.h"

AdjacencyMatrix::AdjacencyMatrix() : m_matrix(nullptr), m_sideSize(0) {}

AdjacencyMatrix::AdjacencyMatrix( const std::string filePath ) : AdjacencyMatrix() {
    if (!readDataFromFile(filePath)) {
        std::cout << "Odczyt z podanego pliku nie powiódł się." << std::endl;
        return;
    }
}

AdjacencyMatrix::~AdjacencyMatrix() {
    delete m_matrix;
}

double AdjacencyMatrix::getMatrixValue (const int i, const int j) const {
    return m_matrix[i*m_sideSize + j];
}

size_t AdjacencyMatrix::getSideSize() const {
    return m_sideSize;
}

void AdjacencyMatrix::printMatrix() const {
    for (int i=0; i<m_sideSize; ++i) {
        for (int j=0; j<m_sideSize; ++j) {
            std::cout << std::setw(5) << getMatrixValue(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

bool AdjacencyMatrix::readDataFromFile( const std::string filePath ){
    std::ifstream infile(filePath, std::ifstream::in);

    if (infile.is_open()) {
		infile >> m_sideSize;

		double * m = new double[m_sideSize * m_sideSize];

		for (int i=0; i<m_sideSize; ++i) {
            for (int j=0; j<m_sideSize; ++j ){
				infile >> m[i*m_sideSize + j];
			}
		}

        m_matrix = m;

        infile.close();
    }
    else {
        std::cout << "Plik " << filePath << " nie istnieje." << std::endl;
        return false;
    }

    return true;
}