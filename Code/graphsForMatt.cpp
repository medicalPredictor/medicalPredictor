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
vector<vector<int> > pathGraph(int n);
vector<vector<int> > randomGraph(int n, double p);
vector<vector<int> > tree(int n, int m);
vector<vector<int> > randomTree(int n, double p);
vector<vector<int> > pathWithCliques(int n, vector<int> m);
void printAdj(vector<vector<int> > adjM);


//Generates a complete graph with n nodes
vector<vector<int> > completeGraph(int n) {
    vector<vector<int> > connectedAdj;
    for (int i = n-1; i > 0; i--) {
        vector<int> node;
        for (int j = 0; j < i; j++)
            node.push_back(1);
        connectedAdj.push_back(node);
    }
    return(connectedAdj);
}

//Generates a path of length n
vector<vector<int> > pathGraph(int n) {
    vector<vector<int> > pathAdj;
    for (int i = n-1; i > 0; i--) {
        vector<int> node;
        int k = 0; // ensures path, every node i needs its ith +1 index to be 1
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

//Generates a graph with n members each having a 1 - p probability of having an edge
vector<vector<int> > randomGraph(int n, double p) {
    vector<vector<int> > randAdj;
    double randNum;
    srand48((int) time(nullptr));
    for (int i = n-1; i > 0; i--) {
        vector<int> node;
        for (int j = 0; j < i; j++) {
            randNum = drand48();
            if (randNum < p)
                node.push_back(1);
            else
                node.push_back(0);
        }
        randAdj.push_back(node);
    }
    return(randAdj);
}

//Generates a tree with n members of depth m as best as possible
vector<vector<int> > tree(int n, int m) {
    vector<vector<int> > treeAdj;
    int parition = lrand48() % n;
    while (parition < n) {
        /* code */
    }
    
    return(treeAdj);
}


//Generates a tree with n members by randomly partitioning the number of nodes
vector<vector<int> > randomTree(int n) {
    vector<vector<int> > rTreeAdj;
    srand48((int) time(nullptr));
    int parition = (lrand48() % (n-1)) + 1;
    int oldPart = 0;
    for (int i = n-1; i > 0; i--) {
        vector<int> node;
        for (int j = 0; j < i; j++) {
            if ((oldPart-1) < j && j <= parition)
                node.push_back(1);
            else
                node.push_back(0);
        }
        oldPart = parition;
        parition = (lrand48() % (n-1-parition)) + 1 + parition;
    }
    return(rTreeAdj);
}


//Generates a path of length n with <m> sized cliques evenly spaced across
vector<vector<int> > pathWithCliques(int n) {
    vector<vector<int> > pathAdj;
    return(pathAdj);
}


void printAdj(vector<vector<int> > adjM) {
    for (int i = 0; i < adjM.size(); i++) {
        for (int j = 0; j < adjM[i].size(); j++)
            cout << adjM[i][j] << " ";
        cout << endl;
    }    
}

void makeBitString(vector<vector<int> > adjM) {
    for (int i = 0; i < adjM.size(); i++) {
        for (int j = 0; j < adjM[i].size(); j++)
            cout << adjM[i][j] << " ";
        cout << endl;
    }    
}

int main() {
    int size = 8; // general number of nodes
    double prob = 0.5; // general probability
    char genericPrint[] = "Strict uppertriangle for";

    vector<vector<int> > connected;
    connected= completeGraph(size);
    printf("%s a complete graph with %d nodes\n", genericPrint, size);
    printAdj(connected);

    vector<vector<int> > path;
    path = pathGraph(size);
    printf("%s a path graph with %d nodes\n", genericPrint, size);
    printAdj(path);

    vector<vector<int> > randy;
    randy = randomGraph(size, prob);
    printf("%s a random graph of size %d and edge probability of %f \n", genericPrint, size, prob);
    printAdj(randy);

    // DOES NOT WORK YET
    vector<vector<int> > randyTree;
    randyTree = randomTree(size);
    printf("%s a random tree graph of size %d \n", genericPrint, size);
    printAdj(randyTree);


    return 0;
}
