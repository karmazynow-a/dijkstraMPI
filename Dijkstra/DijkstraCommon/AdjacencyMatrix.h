#pragma once

#include <vector>
#include <string>
#include <memory>

/// <summary>
/// This class is responsible for loading from file and storing
/// information about processed graph. 
/// </summary>
class AdjacencyMatrix final {

public:

	/// <summary> 
	/// Static factory method used for creating instance of adjacency matrix. 
	/// </summary>
	/// <returns>
	/// Unique pointer that points to created matrix. If data file could not 
	/// be opened, created matrix contains no data.
	/// </returns>
	/// <param name="filePath">
	/// String with path to file that contains adjacency matrix information.
	/// </param>
	static inline std::unique_ptr<AdjacencyMatrix> fromFile(const std::string& filePath) {
		return std::unique_ptr<AdjacencyMatrix>( new AdjacencyMatrix(filePath) );
	}


	/// <summary>
	/// Creates string representation of adjacency matrix. Rows are separated
	/// using <c>std::endl</c> </summary>
	/// <returns>
	/// String with matrix representation.
	/// </returns>
	std::string toString() const;


	/// <summary>
	/// Returns matrix data in a form of continous 1D vector that contains
	/// consecutive matrix columns.
	/// </summary>
	/// <returns>
	/// Vector of doubles that represents matrix data. Data in this vector
	/// is arranged column-wise. 
	/// <example>
	/// If we have matrix like this:
	/// <code>
	/// [ 2 3 4 ]
	/// [ 1 1 3 ]
	/// [ 0 1 0 ]
	/// </code>
	/// Result will look like this: [ 2 1 0 3 1 1 4 3 0 ]
	/// </example>
	/// </returns>
	inline const std::vector<double>& asContinousVector() const {
		return m_matrix;
	}


	/// <summary>
	/// Returns number of graph vertices - same number as number of rows
	/// or columns in adjacency matrix
	/// </summary>
	/// <returns>
	/// Integer number that represents number of graph vertices.
	/// </returns>
	inline int getNumberOfVertices() const {
		return m_numberOfVertices;
	}

private:

	/// <summary>
	/// Private method that reads from data file and assigns values from
	/// there to AdjacencyMatrix properties. If file could not be opened,
	/// proper information is printed and nothing happens.
	/// </summary>
	/// <param name="filePath">
	/// String with path to file that contains adjacency matrix information.
	/// </param>
	void readDataFromFile(const std::string& filePath);


	/// <summary>
	/// Adjacency matrix private constructor. It SHOULD NOT be used on it's
	/// own - for creating instances in code, please use static factory method.
	/// </summary>
	/// <param name="filePath">
	/// String with path to file that contains adjacency matrix information.
	/// </param>
	AdjacencyMatrix(const std::string& filePath){
		readDataFromFile(filePath);
	}


	std::vector<double> m_matrix;
	int m_numberOfVertices = 0;

};
