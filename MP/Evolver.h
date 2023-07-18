#include <iostream>
#include <algorithm>
#include "SDA.h"
#include "CovidModeler.h"
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <string>
#include <sys/stat.h>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <cassert>

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

int getCommandLineArgs(char *arguments[]);
int initializeSystem();
int shutdownSystem();
double fitness(SDA &sda);
int makeVariants();
int report(ostream &outStrm);

int getCommandLineArgs(char *arguments[]) {
    string argVal;
    size_t pos;

    pathToProfile = arguments[1];
    argVal = arguments[2];
    numNodes = stoi(argVal, &pos);
    numPotentialEdges = numNodes * (numNodes - 1) / 2;
    sdaOutput.reserve(numPotentialEdges);
    for (int i = 0; i < numPotentialEdges; i++) {
        sdaOutput.push_back(0);
    }
    argVal = arguments[3];
    numRuns = stoi(argVal, &pos);
    argVal = arguments[4];
    populationSize = stoi(argVal, &pos);
    argVal = arguments[5];
    numGenerations = stoi(argVal, &pos);
    argVal = arguments[6];
    tournSize = stoi(argVal, &pos);
    argVal = arguments[7];
    crossoverRate = stod(argVal, &pos);
    argVal = arguments[8];
    mutationRate = stod(argVal, &pos);
    argVal = arguments[9];
    numMutations.first = stoi(argVal, &pos);
    argVal = arguments[10];
    numMutations.second = stoi(argVal, &pos);
    argVal = arguments[11];
    seed = stoi(argVal, &pos);
    return 0;
}

int initializeSystem() {
    srand48(time(nullptr));
    srand48(seed);

    population = new SDA[populationSize];
    fitnessVals = new double[populationSize];
    makeVariants();
    return 0;
}

int initializePopulation() {
    for (int idx = 0; idx < populationSize; idx++) {
        population[idx] = SDA(12, 2, 2, numPotentialEdges);
        fitnessVals[idx] = fitness(population[idx]);
    }
    return 0;
}

int makeVariants() {
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

double fitness(SDA &sda) {
    double accu = 0.0;

    sda.fillOutput(sdaOutput);
    vector<double> avgProfile;
    avgProfile = runRun("thing.cpp", numSimulations, numNodes, variantLength, alphaProb, recovProb, decayProb,
                        deathProb, variantProb, sdaOutput, variants, {}, false);
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
    fitnessVals[tournIdxs[0]] = fitness(population[tournIdxs[0]]);
    population[tournIdxs[1]].fillOutput(sdaOutput);
    fitnessVals[tournIdxs[1]] = fitness(population[tournIdxs[1]]);
}

int shutdownSystem() {
    delete[] population;
    delete[] fitnessVals;
    return 0;
}

vector<double> calcStats(const vector<int> &goodIdxs, bool biggerBetter) {
    double sum = 0.0;
    double bestVal = (biggerBetter ? 0.0 : MAXFLOAT);
    double worstVal = (biggerBetter ? MAXFLOAT : 0.0);

    for (int idx: goodIdxs) {
        sum += fitnessVals[idx];
        if ((biggerBetter && fitnessVals[idx] > bestVal) || (!biggerBetter && fitnessVals[idx] < bestVal)) {
            bestVal = fitnessVals[idx];
        }
        if ((biggerBetter && fitnessVals[idx] < worstVal) || (!biggerBetter && fitnessVals[idx] > worstVal)) {
            worstVal = fitnessVals[idx];
        }
    }

    double mean = sum / (double) goodIdxs.size();
    double stdDevSum = 0.0;
    for (int idx: goodIdxs) {
        stdDevSum += pow(fitnessVals[idx] - mean, 2);
    }
    double stdDev = sqrt(stdDevSum / ((double) goodIdxs.size() - 1.0));
    double CI95 = 1.96 * (stdDev / sqrt((double) goodIdxs.size()));

    return {mean, stdDev, CI95, bestVal, worstVal}; // {mean, stdDev, 95CI, best, worst}
}

int cmdLineIntro() {
    cout << left << setw(4) << "Run";
    cout << left << setw(4) << "Int";
    cout << left << setw(10) << "Mean";
    cout << left << setw(12) << "95%CI";
    cout << left << setw(10) << "StdDev";
    cout << left << setw(12) << "Best" << endl;
    return 0;
}

int report(ostream &outStrm) {
    vector<int> goodIdxs;
    int deaths = 0;

    goodIdxs.reserve(populationSize);
    for (int i = 0; i < populationSize; i++) {
        goodIdxs.push_back(i);
    }

    vector<double> stats = calcStats(goodIdxs, true);
    double mean = stats[0];
    double stdDev = stats[1];
    double CI95 = stats[2];
    double bestVal = stats[3];
    double worstVal = stats[4];

    outStrm << left << setw(10) << mean;
    outStrm << left << setw(12) << CI95;
    outStrm << left << setw(10) << stdDev;
    outStrm << left << setw(12) << bestVal << endl;

//    cout << left << setw(10) << mean;
//    cout << left << setw(12) << CI95;
//    cout << left << setw(10) << stdDev;
//    cout << left << setw(12) << bestVal << endl;
    return 0;
}