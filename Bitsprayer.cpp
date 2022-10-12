#include "Bitsprayer.h"

#define VERBOSE true

Bitsprayer::Bitsprayer() {

    initInput = -1;
    numStates = -1;
    curState = -1;
//    transitions = {{}};
//    responses = {{{}}};
    if (VERBOSE) cout << "Bitsprayer Made" << endl;
    return;
}

Bitsprayer::Bitsprayer(int states) {

    initInput = -1;
    numStates = states;
    curState = -1;
//    transitions = {{}};
//    responses = {{{}}};
    if (VERBOSE) cout << "Bitsprayer Made w " << states << " states" << endl;
    return;
}

Bitsprayer::Bitsprayer(Bitsprayer &other) {
    return;
}

Bitsprayer::~Bitsprayer() {
    return;
}

int Bitsprayer::create(int states) {
    initInput = rand() % 2;
    numStates = states;
    curState = 0;

    vector<int> oneState;
    for (int s = 0; s < numStates; ++s) {
        oneState.clear();
        oneState.push_back(rand() % numStates);
        oneState.push_back(rand() % numStates);
        transitions.push_back(oneState);
    }

    vector<int> oneResponse;
    vector<vector<int> > oneStateResps;
    int respSize;
    for (int s = 0; s < numStates; ++s) {
        oneStateResps.clear();
        for (int t = 0; t < 2; ++t) {
            oneResponse.clear();
            respSize = rand() % 2 + 1;
            for (int i = 0; i < respSize; ++i) {
                oneResponse.push_back(rand() % 2);
            }
            oneStateResps.push_back(oneResponse);
        }
        responses.push_back(oneStateResps);
    }
    if (VERBOSE) cout << "Bitsprayer Created w " << states << " states" << endl;
    return 0;
}

int Bitsprayer::randomize() {
    initInput = (int) rand() % 2;

    vector<int> oneResponse;
    int respSize;
    for (int s = 0; s < numStates; ++s) {
        transitions.at(s).at(0) = rand() % numStates;
        transitions.at(s).at(1) = rand() % numStates;
        for (int t = 0; t < 2; ++t) {
            oneResponse.clear();
            respSize = rand() % 2 + 1;
            for (int i = 0; i < respSize; ++i) {
                oneResponse.push_back(rand() % 2);
            }
            responses.at(s).at(t) = oneResponse;
        }
    }
}

int Bitsprayer::copy(Bitsprayer &other){
    initInput = other.initInput;
    numStates = other.numStates;
    curState = other.numStates;

    transitions = other.transitions;
    responses = other.responses;
}

int Bitsprayer::print() {
    for (int s = 0; s < numStates; ++s) {
        for (int t = 0; t < 2; ++t) {
            cout << s << " -> " << transitions.at(s).at(t) << " [";
            for (int v: responses.at(s).at(t)) {
                cout << " " << v;
            }
            cout << " ]" << endl;
        }
    }
    if (VERBOSE) cout << "Bitsprayer Printed" << endl;
    return 0;
}

int main() {
    Bitsprayer b(10);
    b.create(10);
    b.print();
    b.randomize();
    b.print();

    Bitsprayer b2;
    b2.copy(b);
    b2.print();

    if (VERBOSE) cout << "DONE!" << endl;
    return 0;
}