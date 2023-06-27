#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>
using namespace std;

// KILL THE ABOVE WHEN YOU PUT IN AS HEADER

// Library to generate interesting adj matricies 
// The general graph class only requires the top half of the adj matrix  


vector<vector<int> > completeGraph(int n);
void printAdj(vector<vector<int> > adjM);
vector<vector<int> > pathGraph(int n);


//Generates a complete graph with n nodes
vector<vector<int> > completeGraph(int n) {
    vector<vector<int> > connectedAdj;
    for (int i = n; i > 0; i--) {
        vector<int> node;
        for (int j = 0; j < i; j++) {
            node.push_back(1);
        }
        connectedAdj.push_back(node);
    }
    return(connectedAdj);
}

//Generates a path of length n
vector<vector<int> > pathGraph(int n) {
    vector<vector<int> > pathAdj;
    for (int i = n; i > 0; i--) {
        vector<int> node;
        int k = 1; // ensures path, every node needs its 1 indexed col entry to be 1
        for (int j = 0; j < i; j++) {
            if (j == k)
                node.push_back(1);
            else
                node.push_back(0);
        }
        pathAdj.push_back(node);
    }
    return(pathAdj);
}


void printAdj(vector<vector<int> > adjM) {
    for (int i = 0; i < adjM.size(); i++) {
        for (int j = 0; j < adjM[i].size(); j++)
            cout << adjM[i][j] << " ";
        cout << endl;
    }    
    //printf("%ld",adjM.size());
}


int main() {
    int size = 4;

    vector<vector<int> > connected;
    connected= completeGraph(size);
    printf("Top half of adj matrix for connected graph of size %d \n", size);
    printAdj(connected);

    vector<vector<int> > path;
    path = pathGraph(size);
    printf("Top half of adj matrix for path graph of size %d \n", size);
    printAdj(path);

    
    return 0;
}
