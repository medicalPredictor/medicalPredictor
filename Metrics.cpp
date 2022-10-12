#include "Graph.h"

/**
 * Fitness functions for our evolver.
 * INPUT:
 * intData -> data from the M.P
 * timeScale -> averaging done by real world data at each timestep
 * typeAgg -> type of averaging: 0 == left weighted, 1 == centered weighted, 2 == right weighted
 * OUTPUT: 
 * aggData -> data averaged based upon real world timescale
 */
float ** aggregateData(vector<float> intData, vector<float> timeScale, int typeAgg) {
    vector<float> aggData{intData};
    for (int i=0; i<size(aggData); i++){
        aggData[i] = 0
        for( int j=0; j<timeScale[i]; j++){
            if typeAgg == 0 {
            // Force it to spit out 0 if not enough data points
                if i-j <0{
                    sum = 0
                    break;
                } else {
                    sum += intData[i-j];
                }
            }
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