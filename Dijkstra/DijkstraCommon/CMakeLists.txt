CMAKE_MINIMUM_REQUIRED( VERSION 3.0.0 )

# Create DijkstraCommon static library
SET( LIB_NAME DijkstraCommon )

SET( LIB_PATH ${CMAKE_SOURCE_DIR}/Dijkstra/DijkstraCommon )

IF( TARGET ${LIB_NAME} )
    message( "${LIB_NAME} is already defined" )
ELSE()
    SET( LIB_HEADERS 
        ${LIB_PATH}/AdjacencyMatrix.h 
        ${LIB_PATH}/CommandLineArgumentsExtractor.h
        ${LIB_PATH}/DijkstraAlgorithmBackend.h
        ${LIB_PATH}/Log.h
        ${LIB_PATH}/ResultsPrinter.h
        ${LIB_PATH}/Validator.h 
        ${LIB_PATH}/VertexData.h )

    SET( LIB_SOURCES 
        ${LIB_PATH}/AdjacencyMatrix.cpp 
        ${LIB_PATH}/DijkstraAlgorithmBackend.cpp 
        ${LIB_PATH}/ResultsPrinter.cpp 
        ${LIB_PATH}/Validator.cpp )

    ADD_LIBRARY( ${LIB_NAME} STATIC
        ${LIB_SOURCES} ${LIB_HEADERS} )

    TARGET_INCLUDE_DIRECTORIES( ${LIB_NAME} PUBLIC
        ${LIB_PATH} )

ENDIF()
