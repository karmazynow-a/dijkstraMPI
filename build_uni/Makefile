
.PHONY: all clean docs install


all: buildMPI buildSerial 
	
buildLib:
	@echo "Building library DijkstraCommon..."
	@$(MAKE) -C ../Dijkstra/DijkstraCommon/

buildSerial: buildLib
	@echo "Building DijkstraSerial..."
	@$(MAKE) -C ../Dijkstra/DijkstraSerial/

buildMPI: buildLib
	@echo "Building DijkstraMPI..."
	@$(MAKE) -C ../Dijkstra/DijkstraMPI/

install: installMPI installSerial

installMPI: buildMPI
	@echo "Installing DijkstraMPI..."
	@cp ../Dijkstra/DijkstraMPI/DijkstraMPI .

installSerial: buildSerial
	@echo "Installing DijkstraSerial..."
	@cp ../Dijkstra/DijkstraSerial/DijkstraSerial .

clean:
	@echo "Cleaning the directory..."
	@cd ../Dijkstra/DijkstraCommon/ && $(MAKE) clean
	@cd ../Dijkstra/DijkstraMPI/ && $(MAKE) clean
	@cd ../Dijkstra/DijkstraSerial/ && $(MAKE) clean
	@rm -f DijkstraMPI DijkstraSerial
	@rm -f resultsMPI.txt resultsSerial.txt
	@rm -rf docs/

docs:
	@echo "Generating docs..."
	@doxygen Doxyfile_uni.in

runMPI: installMPI
	@echo "Running DijkstraMPI..."
	@if [ -z $$VERTEX ] ;\
        then \
            echo "If you want to pass own vertex add argument to command: make runMPI VERTEX=?" ;\
            echo "Running with default value: make runMPI VERTEX=0" ;\
            echo "" ;\
            VERTEX=0 ;\
    fi ;\
    if [ -z $$FILE ] ;\
        then \
            echo "If you want to pass own file add argument to command: make runMPI FILE=?" ;\
            echo "Running with default value: make runMPI FILE=''../data/graph.dat''" ;\
            echo "" ;\
            FILE="../data/graph.dat" ;\
    fi ;\
    if [ -z $$N ] ;\
        then \
            echo "If you want to specify number of processes add argument to command: make runMPI N=?" ;\
			echo "Running with default value: make runMPI N=1" ;\
            echo "" ;\
            N=1 ;\
    fi ;\
    if [ -z $$HOSTS ] ;\
        then \
            echo "If you want to host file add argument to command: make runMPI HOSTS=?" ;\
            echo "Running without hosts file" ;\
            echo "" ;\
            mpiexec -n $$N ./DijkstraMPI $$VERTEX $$FILE ;\
        else \
            mpiexec -f $$HOSTS -n $$N ./DijkstraMPI $$VERTEX $$FILE ;\
    fi \

runSerial: installSerial
	@echo "Running DijkstraSerial..."
	@if [ -z $$VERTEX ] ;\
        then \
            echo "If you want to pass own vertex add argument to command: make runSerial VERTEX=?" ;\
            echo "Running with default value: make runSerial VERTEX=0" ;\
            echo "" ;\
            VERTEX=0 ;\
    fi ;\
    if [ -z $$FILE ] ;\
        then \
            echo "If you want to pass own file add argument to command: make runSerial FILE=?" ;\
            echo "Running with default value: make runSerial FILE=''../data/graph.dat''" ;\
            echo "" ;\
            FILE="../data/graph.dat" ;\
    fi ;\
    ./DijkstraSerial $$VERTEX $$FILE \
