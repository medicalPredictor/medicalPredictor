#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>

// Library to generate interesting adj matricies 
// The general graph class only requires the top half of the adj matrix  

using std::vector;
using std::cout;

vector<vector<int> > connectedGraph(int x, int y);
void printAdj(vector<vector<int> > adjM);

//Connected graph maker, simple the adj is just all 1s
vector<vector<int> > connectedGraph(int numberofNodes) {
    vector<vector<int> > connectedAdj;
    for (int i = numberofNodes; i > 0; i--) {
        vector<int> node;
        for (int j = 0; j < i; j++) {
            node.push_back(1);
        }
        connectedAdj.push_back(node);
    }
    return(connectedAdj);
}

void printAdj(vector<vector<int> > adjM) {
    for (int i = 0; i < adjM.size(); i++) {
        for (int j = 0; j < adjM[i].size(); j++) {
            cout << adjM[i][j] << " ";
        }   
    }    
    //printf("%ld",adjM.size());
}


int main() {
    vector<vector<int> > connected;
    connected= connectedGraph(3);
    printAdj(connected);
    return 0;
}
