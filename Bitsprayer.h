#ifndef MEDICALPREDICTOR_BITSPRAYER_H
#define MEDICALPREDICTOR_BITSPRAYER_H

#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>

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

  private:
    int initInput;
    int numStates;
    int curState;
    vector<vector<int> > transitions;
    vector<vector<vector<int> > > responses;
};


#endif //MEDICALPREDICTOR_BITSPRAYER_H
