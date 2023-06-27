#include <iostream>
#include <algorithm>
#include "SDA.h"
#include "CovidModeler.h"

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
int numPotentialEdges = 100;
int numSimulations = 30;
double alphaProb = 0.63;
double recovProb = 0.126;
double decayProb = 0.006666;
double deathProb = 0.00168214;
double variantProb = 0.0;

int numVariants = 100;
int variantLength = 10;
vector<vector<int>> variants(numVariants);

vector<int> sdaOutput;
SDA *population;
double *fitnessVals;

int getCommandLineArgs(char *arguements[]);
int initializeSystem();
int shutdownSystem();
double fitness(SDA &sda);
int makeVariants();

int getCommandLineArgs(char *arguements[]){
    string argVal;
    size_t pos;

    pathToProfile = arguements[1];
    argVal = arguements[2];
    numNodes = stoi(argVal, &pos);
    numPotentialEdges = numNodes * (numNodes-1)/2;
    sdaOutput.reserve(numPotentialEdges);
    for (int i = 0; i < numPotentialEdges; i++){
        sdaOutput.push_back(0);
    }
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
    return 0;
}

int initializeSystem(){
    srand48(time(nullptr));
    srand48(seed);

    population = new SDA[populationSize];
    fitnessVals = new double[populationSize];
    makeVariants();
    return 0;
}

int initializePopulation(){
    for (int idx=0; idx < populationSize; idx++){
        population[idx] = SDA(12, 2, 2, numPotentialEdges);
        fitnessVals[idx] = fitness(population[idx]);
        cout<<"Initialized One Dude"<<endl;
    }
    return 0;
}

int makeVariants(){
    vector<int> oneVariant;
    oneVariant.reserve(variantLength);
    for (int var = 0; var < numVariants; ++var) {
        oneVariant.clear();
        for (int bit = 0; bit < variantLength; ++bit) {
            oneVariant.push_back(lrand48() % 2);
        }
        variants[var] = oneVariant;
    }
    return 0;
}

double fitness(SDA &sda){
    double accu = 0.0;

    sda.fillOutput(sdaOutput);
    vector<double> avgProfile;
    avgProfile = runRun("thing.cpp", 30, numNodes, variantLength, alphaProb, recovProb, decayProb, deathProb, variantProb, sdaOutput, variants, {}, false);
    for (int i = 0; i < avgProfile.size(); ++i) {

        accu += avgProfile[i];
    }
    return accu;
}

bool compareFitness(int popIdx1, int popIdx2) {
    return fitnessVals[popIdx1] < fitnessVals[popIdx2];
}

vector<int> tournSelect(int size, bool decreasing) {
    vector<int> tournIdxs;
    int idxToAdd;

    tournIdxs.reserve(size);
    if (size == populationSize) {
        for (int idx = 0; idx < size; idx++) {
            tournIdxs.push_back(idx);
        }
    } else {
        do {
            idxToAdd = (int) lrand48() % populationSize;
            if (count(tournIdxs.begin(), tournIdxs.end(), idxToAdd) == 0) {
                tournIdxs.push_back(idxToAdd);
            }
        } while (tournIdxs.size() < tournSize);
    }

    sort(tournIdxs.begin(), tournIdxs.end(), compareFitness);
    if (decreasing) {
        reverse(tournIdxs.begin(), tournIdxs.end());
    }
    return tournIdxs;
}

void matingEvent() {
    int numMuts;
    vector<int> tournIdxs;
    // Selection
    tournIdxs = tournSelect(tournSize, false);

    // Copy the Parents -> Children
    population[tournIdxs[0]].copy(population[tournIdxs[tournSize - 2]]);
    population[tournIdxs[1]].copy(population[tournIdxs[tournSize - 1]]);

    // Crossover
    if (drand48() < crossoverRate) population[tournIdxs[0]].twoPointCrossover(population[tournIdxs[1]]);

    // Mutation
    int mutRange = numMutations.second - numMutations.first;
    if (drand48() < mutationRate) {
        numMuts = (int) lrand48() % mutRange + numMutations.first;
        population[tournIdxs[0]].mutate(numMuts);
        numMuts = (int) lrand48() % mutRange + numMutations.first;
        population[tournIdxs[1]].mutate(numMuts);
    }

    // Reset dead SDAs
    population[tournIdxs[0]].fillOutput(sdaOutput);
    population[tournIdxs[1]].fillOutput(sdaOutput);
}

int shutdownSystem(){
    delete[] population;
    delete[] fitnessVals;
    return 0;
}