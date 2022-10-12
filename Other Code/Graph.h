#include <vector>
#include <bitset>
#include <iostream>

#ifndef MEDICALPREDICTOR_GRAPH_H
#define MEDICALPREDICTOR_GRAPH_H

using namespace std;

/**
 * This class defines a Graph G(V, E) which contains a set of vertices V and a set of edges
 * between vertices E.  These Graphs will be used to simulate epidemics on social contact
 * networks (graphs of physical connections between individuals).
 */
class Graph {
  public:
    int initialize(int numNodes, vector<bool> bitspray);
    int printAdjLists();

  private:
    int numNodes;
    vector<vector<bool> > adj;
};


#endif //MEDICALPREDICTOR_GRAPH_H
