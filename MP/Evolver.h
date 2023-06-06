#include <iostream>
#include "SDA.h"

using namespace std;

string pathToProfile;
int numNodes;
int numRuns;
int populationSize;
int numGenerations;
int tournSize;
double crossoverRate;
double mutationRate;
pair<int, int> numMutations;
int seed;

SDA *population;
double *fitnessVals;

int getCommandLineArgs(char *arguements[]);
int initializeSystem();
int shutdownSystem();

int getCommandLineArgs(char *arguements[]){
    string argVal;
    size_t pos;

    pathToProfile = arguements[1];
    argVal = arguements[2];
    numNodes = stoi(argVal, &pos);
    argVal = arguements[3];
    numRuns = stoi(argVal, &pos);
    argVal = arguements[4];
    populationSize = stoi(argVal, &pos);
    argVal = arguements[5];
    numGenerations = stoi(argVal, &pos);
    argVal = arguements[6];
    tournSize = stoi(argVal, &pos);
    argVal = arguements[7];
    crossoverRate = stod(argVal, &pos);
    argVal = arguements[8];
    mutationRate = stod(argVal, &pos);
    argVal = arguements[9];
    numMutations.first = stoi(argVal, &pos);
    argVal = arguements[10];
    numMutations.second = stoi(argVal, &pos);
    argVal = arguements[11];
    seed = stoi(argVal, &pos);
}

int initializeSystem(){
    srand48(time(nullptr));
    srand48(seed);

    population = new SDA[populationSize];
    fitnessVals = new double[populationSize];
    return 0;
}

int shutdownSystem(){
    delete[] population;
    delete[] fitnessVals;
    return 0;
}