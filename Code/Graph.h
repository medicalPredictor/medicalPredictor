#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>
//#include "GraphGen.h"

using namespace std;

class Graph {
public:
    // functions go here
    int initialize(int nn1, vector<int> bitspray, int immunityStringSize, vector<int> bitspray2, double alpha1,
                   double recProb1, double decProb1, double
                   deathProb1, double variantProb1);

    int addVariant(vector<int> bits, int len);
    int printVariantList();
    int printImmunityList();
    int printVariantHistory();
    int infect(int node, int variant);
    vector<int> getState();
    int unInfect(int node);
    int removeImmunity(int node);
    vector<double> infectionProb(vector<int> Ac, int variant);
    vector<int> calcAc(int variant);
    int runInfections(vector<int> new1, int variant);
    int recoveries();
    int death(int node);
    int ImmunityDecay();
    vector<int> getInfected();
    vector<vector<int> > getCurrentVariantHistory();
    int deathCount();
	int newInfectionCount();
	int newDeathCount();
	int newRecoveriesCount();
	vector <int> getnewVariantInfectionsCount();
    int lifeCount();
    vector<int> runVariant(int variant);
    int updateInfected();
    void printAdj();
    int newVariant(vector<int> bits);
    void kill();
    vector<int> nextTimeStep();
    int numOfVariants();
    vector<int> simulation(string outfile, std::vector < vector<int> > variantList, int * final1, vector < int > newInfectedDate, vector < vector < int > > * TotalNewVariantInfectedLog, bool FileFlag);
	void printVariantLog(std::vector < vector<int> > variantList, string outfile, vector < int > newInfectedDate, vector < vector < int > > * TotalNewVariantInfectedLog, int * final1);
	void printLog(vector<int> TotalNewInfectLog, string outfile);
	int printVector(vector<int> vec);
	int printVector(vector<int> vec, ofstream & vals);
	void ReadData(string input1, std::vector<int> *listOfPoints, int linesize);
	void ReadData2(string input1, std::vector <std::vector<int> > *listOfPoints, int linesize);
	void tokenize(std::string const str, const char delim, std::vector<int> *out);

private:
    //Variables go here
    double alpha;       // The likelihood of passing on an infection along one edge
    double recProb;     // Daily likelihood of recovering (while infected)
    double decProb;     // Daily likelihood of losing immunity (state -1 or 0)
    double deathProb;   // Daily likelihood of dying (while infected)
	double variantProb;   // Daily likelihood of generating a new Variant
    vector<vector<int> > adj;  // Adjacency matrix
    int nn; // Number of nodes
    int immunitySize; // Size of the immunity and variant string
    // state[n] = >0: variant index + 1 n is infected with, 0: susceptible w no immunity, -1: has immunity to
    // something, -2: dead
    vector<int> state;
    // infected[n] = 1: n is currently infected, 0: n is currently not infected
    vector<int> infected;
	vector<int> newinfectedbyVariant;
    // variantDic[n] = {a record of all the variants generated since the start of the epi, the oldest first}
    vector<vector<int> > VariantDic;
    // immunityDic[n] = {n's current immunity string where >1 means immunity and 0 means susceptible}
    vector<vector<int> > immunityDic;
    // variantHistory[n] = {queue of variants indices + 1 n has been infected with and still has immunity to, the
    // oldest first}
    vector<vector<int> > variantHistory;
    int deathcount; // Total deaths during epi
    double calcExtra(vector<int> Ac, int x, int totalBits, int variant);
    int newInfections;
	int newDeaths;
	int newRecoveries;
};
