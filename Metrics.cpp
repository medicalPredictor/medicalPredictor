#include "Graph.h"

/**
 * Fitness functions for our evolver.
 */
float ** aggregateData(vector<float> intData ) {
    aggData = 
    aggTimeScale = vector<float>
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