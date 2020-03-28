#pragma once

class AdjacencyMatrix final {

public:
/*
	double test_matrix[TEST_MATRIX_SIDE][TEST_MATRIX_SIDE] = {
		//{0, 0, 9, 0, 7, 0, 0, 0},
		//{0, 0, 0, 0, 0, 0, 2, 0},
		//{9, 0, 0, 0, 0, 9, 0, 0},
		//{0, 0, 0, 0, 0, 0, 0, 0},
		//{7, 0, 0, 0, 0, 0, 0, 0},
		//{0, 0, 9, 0, 0, 0, 3, 6},
		//{0, 2, 0, 0, 0, 3, 0, 5},
		//{0, 0, 0, 0, 0, 6, 5, 0}

		{ 0, 4, 4, 0, 0, 0, 0, 0 },
		{ 2, 0, 7, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 3, 0, 0, 0, 7, 0 },
		{ 0, 0, 0, 0, 0, 0, 5, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 7, 0, 0 }
	};
*/
	AdjacencyMatrix ();
	AdjacencyMatrix ( const std::string filePath );
	~AdjacencyMatrix ();

	double getMatrixValue (const int i, const int j) const;
	size_t getSideSize () const;
	void printMatrix () const;

private:
	bool readDataFromFile( const std::string filePath );

	double* m_matrix;
	size_t m_sideSize;

};