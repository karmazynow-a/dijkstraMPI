CMAKE_MINIMUM_REQUIRED( VERSION 3.0.0 )

# Logging
ADD_DEFINITIONS( -DSHOULD_LOG=true )

# Include DijkstraCommon library
SET( COMMON_PATH /Dijkstra/DijkstraCommon )
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}${COMMON_PATH} )

# Include DijkstraMPI
SET( MPI_PATH /Dijkstra/DijkstraMPI )
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}${MPI_PATH} )

# Include DijkstraSerial
SET( SERIAL_PATH /Dijkstra/DijkstraSerial )
ADD_SUBDIRECTORY( ${CMAKE_SOURCE_DIR}${SERIAL_PATH} )

# Doxygen
FIND_PACKAGE(Doxygen)
OPTION(BUILD_DOC "Build documentation" OFF)

IF (BUILD_DOC)
    IF( DOXYGEN_FOUND )
        # set input and output files
        SET( DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in )
        SET( DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/build/Doxyfile )

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
ENDIF (BUILD_DOC)

# Clean
SET( cmake_generated
    ${CMAKE_BINARY_DIR}/CMakeCache.txt
    ${CMAKE_BINARY_DIR}/CMakeDoxyfile.in
    ${CMAKE_BINARY_DIR}/CMakeDoxygenDefaults.cmake
    ${CMAKE_BINARY_DIR}/docs
    ${CMAKE_BINARY_DIR}/Doxyfile
    ${CMAKE_BINARY_DIR}/resultsSerial.txt
    ${CMAKE_BINARY_DIR}/resultsMPI.txt
    ${CMAKE_BINARY_DIR}/cmake_install.cmake
    ${CMAKE_BINARY_DIR}/Makefile
    ${CMAKE_BINARY_DIR}/CMakeFiles
    ${CMAKE_BINARY_DIR}${MPI_PATH}/cmake_install.cmake
    ${CMAKE_BINARY_DIR}${MPI_PATH}/Makefile
    ${CMAKE_BINARY_DIR}${MPI_PATH}/CMakeFiles
    ${CMAKE_BINARY_DIR}${SERIAL_PATH}/cmake_install.cmake
    ${CMAKE_BINARY_DIR}${SERIAL_PATH}/Makefile
    ${CMAKE_BINARY_DIR}${SERIAL_PATH}/CMakeFiles
    ${CMAKE_BINARY_DIR}${COMMON_PATH}/cmake_install.cmake
    ${CMAKE_BINARY_DIR}${COMMON_PATH}/Makefile
    ${CMAKE_BINARY_DIR}${COMMON_PATH}/CMakeFiles
    ${CMAKE_BINARY_DIR}/*DijkstraSerial
    ${CMAKE_BINARY_DIR}/*DijkstraMPI
    ${CMAKE_BINARY_DIR}/*libDijkstraCommon.a  )

ADD_CUSTOM_TARGET( 
    clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND for f in ${cmake_generated} \; do rm -rf $$f \; done
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning the directory" )

# Install
ADD_CUSTOM_TARGET( 
    install-all
    COMMAND cp ${CMAKE_BINARY_DIR}${MPI_PATH}/DijkstraMPI .
    COMMAND cp ${CMAKE_BINARY_DIR}${SERIAL_PATH}/DijkstraSerial .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS DijkstraMPI DijkstraSerial
    COMMENT "Run DijkstraSerial" )

# Run
ADD_CUSTOM_TARGET( 
    runMPI
    COMMAND if [ -z "$$VERTEX" ] \; 
        then 
            echo "If you want to pass own vertex add argument to command: make runMPI VERTEX=?" \; 
            echo "Running with default value: make runMPI VERTEX=0" \; 
            echo "" \;
            VERTEX="0" \;
        fi \;
        if [ -z "$$FILE" ] \;
        then 
            echo "If you want to pass own file add argument to command: make runMPI FILE=?" \; 
            echo "Running with default value: make runMPI FILE=''../data/graph.dat''" \; 
            echo "" \;
            FILE="../data/graph.dat" \;
        fi \;
        if [ -z "$$N" ] \;
        then 
            echo "If you want to specify number of processes add argument to command: make runMPI N=?" \; 
            echo "Running with default value: make runMPI N=1" \; 
            echo "" \;
            N=1 \;
        fi \;
        if [ -z "$$HOSTS" ] \;
        then 
            echo "If you want to host file add argument to command: make runMPI HOSTS=?" \; 
            echo "Running without hosts file" \; 
            echo "" \;
            mpiexec -n $$N ${CMAKE_CURRENT_BINARY_DIR}${MPI_PATH}/DijkstraMPI $$VERTEX $$FILE \;
        else
            mpiexec -f $$HOSTS -n $$N ${CMAKE_CURRENT_BINARY_DIR}${MPI_PATH}/DijkstraMPI $$VERTEX $$FILE \;
        fi \;

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run DijkstraMPI" )

ADD_CUSTOM_TARGET( 
    runSerial
    COMMAND if [ -z "$$VERTEX" ] \; 
        then 
            echo "If you want to pass own vertex add argument to command: make runSerial VERTEX=?" \; 
            echo "Running with default value: make runSerial VERTEX=0" \; 
            echo "" \;
            VERTEX="0" \;
        fi \;
        if [ -z "$$FILE" ] \;
        then 
            echo "If you want to pass own file add argument to command: make runSerial FILE=?" \; 
            echo "Running with default value: make runSerial FILE=''../data/graph.dat''" \; 
            echo "" \;
            FILE="../data/graph.dat" \;
        fi \;
        ${CMAKE_CURRENT_BINARY_DIR}${SERIAL_PATH}/DijkstraSerial $$VERTEX $$FILE \;

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run DijkstraSerial" )