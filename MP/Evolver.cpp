#include "Evolver.h"

/**
 * This method will evolve a population of SDAs.  Each SDA represents a particular personal contact network.
 * The goal of the evolved network will be to match a normalized epidemic profile from a larger network.
 * 
 * The command line parameters are as follows:
 * 1. Path to Original Epidemic Profile
 * 2. Number of Nodes in Network
 * 3. Number of Runs
 * 4. Population Size
 * 5. Number of Generations
 * 6. Tournament Size
 * 7. Crossover Rate
 * 8. Mutation Rate
 * 9. Minimum Number of Mutations
 * 10. Maximum Number of Mutations
 * 11. Seed for Random Number Generator
*/
int main(int numArgs, char *arguments[]){
    getCommandLineArgs(arguments);
    initializeSystem();
    cmdLineIntro();

    for (int run = 1; run < numRuns + 1; run++){
        initializePopulation();
        cout << left << setw(4) << run;
        cout << left << setw(4) << 0;
        report(cout);
        for (int mev = 0; mev < numGenerations; ++mev) {
            matingEvent();
            if (mev > 0 && (0 == mev % (numGenerations/100))){
                cout << left << setw(4) << run;
                cout << left << setw(4) << (int)(mev / (numGenerations/100));
                report(cout);
            }
        }
        cout << left << setw(4) << run;
        cout << left << setw(4) << 100;
        report(cout);
    }

    shutdownSystem();
    return 0;
}