
.PHONY: all clean docs


all:
	$(MAKE) -C ../Dijkstra/DijkstraCommon/
	$(MAKE) -C ../Dijkstra/DijkstraMPI/
	#$(MAKE) -C ../Dijkstra/DijkstraSerial/

clean:
	cd ../Dijkstra/DijkstraCommon/ && $(MAKE) clean
	cd ../Dijkstra/DijkstraMPI/ && $(MAKE) clean
	cd ../Dijkstra/DijkstraSerial/ && $(MAKE) clean

docs:
	doxygen -g ../Doxyfile.in