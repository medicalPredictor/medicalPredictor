#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>

#define VERBOSE true
using namespace std;

class Bitsprayer {
public:
    Bitsprayer();           //creates an unallocated bitspray
    Bitsprayer(int states);      //create a bitspray with buffer S states
    Bitsprayer(Bitsprayer &other);  //copy constructor
    ~Bitsprayer();                //destructor

    int create(int states);

    int randomize();

    int copy(Bitsprayer &other);

    int print();

    static int destroy();

    int twoPtCrossover(Bitsprayer &other);

    int mutate(int numMuts);

    vector<int> getBitsVec(int len);

private:
    int initInput;
    int numStates;
    int initState;
    int curState;
    vector<int> buf;
    vector<vector<int> > transitions;
    vector<vector<vector<int> > > responses;
};
