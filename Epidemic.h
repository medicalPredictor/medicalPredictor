//
// Created by Michael Dubé on 2022-09-15.
//

#ifndef MEDICALPREDICTOR_EPIDEMIC_H
#define MEDICALPREDICTOR_EPIDEMIC_H

#include "Graph.h"

class Epidemic {
  public:
    int initialize(Graph network, double alpha, int immuneSize, vector<bool> initVariant, double probIS, double probXS, double probIR);

  private:
    Graph network;
    double alpha;
    int immuneSize;
    vector<vector<bool> > variantDic;
    // The following probabilities use the Susceptible-Infected-Removed (SIR) model.  X stands for any state.
    double probIS;  // Probability of recovering from infection (I -> S)
    double probXS;  // Probability of losing immunity (X -> S)
    double probIR;  // Probability of dying from infection (I -> R)
//    vector<int> state;
//    vector<int> infected;
//    vector<vector<int> > immunityDic;
//    vector<vector<int> > variantHistory;
//    int deathcount;
};


#endif //MEDICALPREDICTOR_EPIDEMIC_H
