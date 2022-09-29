#include "Graph.h"

/**
 * Initializes a graph object with the following settings.
 *
 * @param numNodes  Number of nodes.
 * @param bitspray  Vector of integers representing the adjacency matrix of the graph.  TODO: replace with bitsprayer object.
 * @return          Keep the system happy.
 */
int Graph::initialize(int numNodes, vector<bool> bitspray) {
    this->numNodes = numNodes;

    // Create a row of false (zero) for adjacency and fill adj matrix
    vector<bool> holder(numNodes, false);
    for (int y = 0; y < numNodes; y++) {
        adj.push_back(holder);
    }

    int counter=0;
    for (int x = 0; x < adj.size(); x++) {
        for (int y = x + 1; y < adj[x].size(); y++) {
            adj[x][y] = bitspray[counter];
            adj[y][x] = bitspray[counter];
            counter++;
        }
    }
    return (0);
}

/**
 * Prints the adjacency lists representing the network.  Each line starts with the node followed
 * by a tab character.  Then each node it has adjacency to is printed, separated by a space.
 *
 * @param numNodes  Number of nodes.
 * @param bitspray  Vector of integers representing the adjacency matrix of the graph.  TODO: replace with bitsprayer object.
 * @return          Keep the system happy.
 */
int Graph::printAdjLists() {
    for (int from=0; from<numNodes; from++){
        cout<<from<<"\t";
        for (int to=0; to<numNodes; to++){
            if (to != from && adj[from][to]){
                cout<<to<<" ";
            }
        }
        cout<<endl;
    }
    return (0);
}