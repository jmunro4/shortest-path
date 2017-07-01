OPTS = -g -Wall -std=c++11 -pthread

all:
	make sptest
	make currency
	make suggestwords
	make mapqueue

sptest: sptest.cc shortestPath.o BinaryHeap.o
	g++ ${OPTS} -o sptest sptest.cc shortestPath.o BinaryHeap.o

currency: currency.cc shortestPath.o BinaryHeap.o
	g++ ${OPTS} -o currency currency.cc shortestPath.o BinaryHeap.o
	
suggestwords: suggestwords.cc
	g++ ${OPTS} -o suggestwords suggestwords.cc

mapqueue: mapqueue.cc
	g++ ${OPTS} -o mapqueue mapqueue.cc

shortestPath.o: shortestPath.cc shortestPath.h
	g++ ${OPTS} -c shortestPath.cc

BinaryHeap.o: BinaryHeap.cc BinaryHeap.h
	g++ ${OPTS} -c BinaryHeap.cc
	
clean:
	rm *.o
	rm sptest
	rm currency
	rm suggestwords
	rm mapqueue
	rm -r *.dSYM
cleanemacs:
	rm *~		
	
