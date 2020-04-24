
.PHONY: all clean docs install


all: buildMPI buildSerial 
	
buildLib:
	$(MAKE) -C ../Dijkstra/DijkstraCommon/

buildSerial: buildLib
	$(MAKE) -C ../Dijkstra/DijkstraSerial/

buildMPI : buildLib
	$(MAKE) -C ../Dijkstra/DijkstraMPI/

install: installMPI installSerial

installMPI: buildMPI
	cp ../Dijkstra/DijkstraMPI/DijkstraMPI .

installSerial: buildSerial
	cp ../Dijkstra/DijkstraSerial/DijkstraSerial .

clean:
	cd ../Dijkstra/DijkstraCommon/ && $(MAKE) clean
	cd ../Dijkstra/DijkstraMPI/ && $(MAKE) clean
	cd ../Dijkstra/DijkstraSerial/ && $(MAKE) clean

docs:
	doxygen ../Doxyfile.in

runMPI: installMPI
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
            mpiexec -n $$N DijkstraMPI $$VERTEX $$FILE ;\
        else \
            mpiexec -n $$N -f $$HOSTS DijkstraMPI $$VERTEX $$FILE ;\
    fi \

runSerial: installSerial
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
            mpiexec -n $$N DijkstraMPI $$VERTEX $$FILE ;\
        else \
            mpiexec -n $$N -f $$HOSTS DijkstraMPI $$VERTEX $$FILE ;\
    fi \