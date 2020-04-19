CMAKE_MINIMUM_REQUIRED( VERSION 3.0.0 )

# Check if proper build directory is being used
GET_FILENAME_COMPONENT(BUILD_DIRECTORY ${CMAKE_BINARY_DIR} NAME)
IF(NOT BUILD_DIRECTORY MATCHES "build")
    MESSAGE(FATAL_ERROR "This project supports only out-of-source building from directory build. Create proper directory: mkdir build and call cmake from it.")
ENDIF()

# Include DijkstraCommon library
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}/Dijkstra/DijkstraCommon )

# Include DijkstraMPI
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}/Dijkstra/DijkstraMPI )

# Include DijkstraSerial
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}/Dijkstra/DijkstraSerial )


# Doxygen
FIND_PACKAGE(Doxygen)
# OPTION(BUILD_DOC "Build documentation" ON)

IF( DOXYGEN_FOUND )
    # set input and output files
    SET( DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in )
    SET( DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/build/Doxyfile )
    SET( DOXYGEN_GENERATE_LATEX NO )
    SET( DOXYGEN_PROJECT_NAME "Dijkstra Algorithm Implementation" )
    SET( DOXYGEN_PROJECT_BRIEF "This project contains various Dijkstra algorithm implementations (serial, MPI)." )

    # request to configure the file
    CONFIGURE_FILE( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )
    MESSAGE( "Doxygen build started" )

    ADD_CUSTOM_TARGET( 
        doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

ELSE(DOXYGEN_FOUND)
  message( "Doxygen need to be installed to generate the doxygen documentation" )

ENDIF(DOXYGEN_FOUND)


# Clean
SET( cmake_generated
    ${CMAKE_BINARY_DIR}/CMakeCache.txt
    ${CMAKE_BINARY_DIR}/CMakeDoxyfile.in
    ${CMAKE_BINARY_DIR}/CMakeDoxygenDefaults.cmake
    ${CMAKE_BINARY_DIR}/cmake_install.cmake
    ${CMAKE_BINARY_DIR}/Dijkstra
    ${CMAKE_BINARY_DIR}/docs
    ${CMAKE_BINARY_DIR}/Doxyfile
    ${CMAKE_BINARY_DIR}/resultsSerial.txt
    ${CMAKE_BINARY_DIR}/resultsMPI.txt
    ${CMAKE_BINARY_DIR}/Makefile
    ${CMAKE_BINARY_DIR}/CMakeFiles )

ADD_CUSTOM_TARGET( 
    clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND for f in ${cmake_generated} \; do rm -rf $$f \; done
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning the directory" )


# Run
ADD_CUSTOM_TARGET( 
    runMPI
    COMMAND if [ -z "$$MPI_VERTEX" ] \; 
        then 
            echo "If you want to pass own vertex add argument to command: make runMPI VERTEX=?" \; 
            echo "Running with default value: make runMPI VERTEX=0" \; 
            ${CMAKE_CURRENT_BINARY_DIR}/Dijkstra/DijkstraMPI/DijkstraMPI 0 \;
        else    
            ${CMAKE_CURRENT_BINARY_DIR}/Dijkstra/DijkstraMPI/DijkstraMPI $$VERTEX \;
        fi

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run DijkstraMPI" )

ADD_CUSTOM_TARGET( 
    runSerial
    COMMAND if [ -z "$$MPI_VERTEX" ] \; 
        then 
            echo "If you want to pass own vertex add argument to command: make runSerial VERTEX=?" \; 
            echo "Running with default value: make runSerial VERTEX=0" \; 
            ${CMAKE_CURRENT_BINARY_DIR}/Dijkstra/DijkstraSerial/DijkstraSerial 0 \;
        else    
            ${CMAKE_CURRENT_BINARY_DIR}/Dijkstra/DijkstraSerial/DijkstraSerial $$VERTEX \;
        fi

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run DijkstraSerial" )