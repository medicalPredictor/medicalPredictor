all: CovidModeler.cpp CovidModeler.h Graph.cpp Graph.h 
	g++ -std=c++11 CovidModeler.cpp Graph.cpp -o CovidModel

test: CovidModeler.cpp CovidModeler.h Graph.cpp Graph.h GraphGen.h
	g++ -std=c++11 CovidModeler.cpp Graph.cpp -o MattGraphTest