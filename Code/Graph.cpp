#include "Graph.h"

/**
 * Initializes a graph object with the following settings.
 *
 * @param nn1  Number of nodes. (int)
 * @param bitspray  Vector of integers representing the adjacency matrix of the graph.
 * @param immunityStringSize  Length of the immunity string. (int)
 * @param bitspray2  Vector of integers representing the first variants immunity string.
 * @param alpha1  Base probability of infection.
 * @param recProb1  Probability of recovering on any particular day.
 * @param decProb1 Probability of immunity decay on any particular day.
 * @return          If the program completes successfully, returns 0.
 */
int Graph::initialize(int nn1, vector<int> bitspray, int immunityStringSize, vector<int> bitspray2, double alpha1, double recProb1, double decProb1, double
deathProb1) {
    nn = nn1;
    int counter = 0;
    vector<int> holder;
    for (int x = 0; x < nn; x++) {
        holder.push_back(0);
        state.push_back(0);
        infected.push_back(0);
    }
    for (int y = 0; y < nn; y++) {
        adj.push_back(holder);
    }
    //cout << "HERE2" << endl;
    //cout<<bitspray[0] << endl;
   // cout<<bitspray.empty()<<endl;

    for (int x = 0; x < adj.size(); x++) {
        for (int y = x + 1; y < adj[x].size(); y++) {
            adj[x][y] = bitspray[counter];
            adj[y][x] = bitspray[counter];
            counter++;
        }
    }

    //cout << "HERE2.2" << endl;

    alpha = alpha1;
    recProb = recProb1;
    decProb = decProb1;
    deathProb = deathProb1; // TODO: Update so that it is an argument.
//    deathProb = 0.00168214; // TODO: Update so that it is an argument.

    immunitySize = immunityStringSize;
    addVariant(bitspray2, immunitySize);

    vector<int> holder2;
    for (int y = 0; y < immunitySize; y++) {
        holder2.push_back(0);
    }

    for (int x = 0; x < nn; x++) {
        immunityDic.push_back(holder2);
    }

    holder.clear();
//	for(int x = holder.size(); x > 0; x--)
//	{
//		holder.pop_back();
//	}

    for (int x = 0; x < nn; x++) {
        variantHistory.push_back(holder);
    }

    deathcount = 0;

    return (0);
}

/**
 * Returns the current number of variants in the simulation
 *
 * @return         Integer representing the total number of variants currently in the simulation.
 */
int Graph::numOfVariants() {
    return (VariantDic.size());
}

/**
 * Updates the Graph object for the next timestep, and returns the number of currently infected nodes, the number of cumulative deaths, and number of currently alive nodes.
 *
 * @return       A vector conatining: the number of currently infected nodes, the number of cumulative deaths, and number of currently alive nodes.
 */
vector<int> Graph::nextTimeStep() {
    //cout << "Inside nextTimeStep\n";
    vector<int> stats;
    int deaths = 0;
    int life = 0;
    vector<int> infected;
    int sumInf = 0;
    updateInfected();
    //cout << "infected updated\n";
    infected = getInfected();
    //cout << "infected aquired\n";
    //printVector(infected);
    for (int x = 0; x < infected.size(); x++) {
        sumInf += infected[x];
    }

    deaths = deathCount();
    life = lifeCount();

    stats.push_back(sumInf);
    stats.push_back(deaths);
    stats.push_back(life);

    return (stats);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string.
 * @return          If the program completes successfully, returns 0.
 */
int Graph::addVariant(vector<int> bits, int len) // TODO: Remove len
{
    vector<int> variant;
    for (int x = 0; x < immunitySize; x++) {
        variant.push_back(bits[x]);
    }
    VariantDic.push_back(variant);
    return (0);
}

/**
 * Prints the immunity string for each node to the commandline.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printImmunityList() {
    cout << "Immunity List:\n";
    for (int x = 0; x < immunityDic.size(); x++) {
        for (int y = 0; y < immunityDic[x].size(); y++) {
            cout << immunityDic[x][y] << " ";
        }
        cout << "\n";
    }
    return (0);
}

/**
 * Prints the Lists of Variants to the commandline.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printVariantList() {
    cout << "Variant List:\n";
    for (int x = 0; x < VariantDic.size(); x++) {
        for (int y = 0; y < VariantDic[x].size(); y++) {
            cout << VariantDic[x][y] << " ";
        }
        cout << "\n";
    }
    return (0);
}

/**
 * Prints the infection variant history for each node. This vector helps keep track of immunity decay, as the first member of this vector is removed during immunity decay. New infections are added at the end.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printVariantHistory() {
    cout << "Variant History List:\n";
    for (int x = 0; x < variantHistory.size(); x++) {
        for (int y = 0; y < variantHistory[x].size(); y++) {
            cout << variantHistory[x][y] << " ";
        }
        cout << "\n";
    }
    return (0);
}

/**
 * Returns a copy of the state vector. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of the state vector.
 */
vector<int> Graph::getState() {
    return (state);
}

/**
 * Returns a copy of the vector of currently infected nodes. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of the vector of currently infected nodes.
 */
vector<int> Graph::getInfected() {
    return (infected);
}

/**
 * Returns a copy of variant history for each node, contained in a vector of vector of ints. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of variant history for each node, contained in a vector of vector of ints.
 */
vector<vector<int> > Graph::getCurrentVariantHistory() {
    return (variantHistory);
}

/**
 * Returns the current death total of the simulation. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          Current death count of the simulation.
 */
int Graph::deathCount() {
    return (deathcount);
}

/**
 * Returns the current number of nodes that are still alive in the simulation. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          The current number of nodes that are still alive in the simulation.
 */
int Graph::lifeCount() {
    return (nn - deathcount);
}

/**
 * Infects the passed node with the passed variant. Updates all internal datastructures to reflect the new information. Checks to ensure the node index is between 0 and total number of nodes. Checks to ensure variant exists. Only infects if the immunity string does not cover all ones in the variant string.
 *
 * @param node      integer representing the index of the node to be infected.
 * @param variant   index of the variant from the variant dictionary that is to infect the node.
 * @return          If the program completes successfully, returns 0. If the node is out of bounds, returns 2. If the variant does not exist in the dictionary, returns 3. Otherwise, returns 1.
 */
int Graph::infect(int node, int variant) {
    if (node < 0 | node >= nn) {
        cout << "Error. Node outside of Bounds.\n";
        return (2);
    }
    if (variant > VariantDic.size()) {
        cout << "Error. Variant Not Found.\n";
        return (3);
    }
    int stuff = 0;
    int counter = 0;
    // TODO: Possibly look into different variant intro?
    if (state[node] == 0 | state[node] == -1) {
        for (int x = 0; x < immunitySize; x++) {
            stuff = VariantDic[variant][x] - immunityDic[node][x];
            if (stuff > 0) {
                counter++;
            }
        }
        if (counter > 0) {
            state[node] = variant + 1;
            infected[node] = 1;
            for (int x = 0; x < immunityDic[node].size(); x++) {
                immunityDic[node][x] = immunityDic[node][x] + VariantDic[variant][x];
            }
            variantHistory[node].push_back(variant + 1);
            return (0);
        }
    }
    return (1);
}

/**
 * Remove infection from the passed node. Updates internal data structures to reflect the new state. Checks to ensure the node index is between 0 and total number of nodes.
 *
 * @param node  Integer representing the index of the node to be infected.
 * @return          If the program completes successfully, returns 0. If the node is out of bounds, returns 2.
 */
int Graph::unInfect(int node) {
    if (node < 0 | node >= nn) {
        cout << "Error. Node outside Bounds.\n";
        return (2);
    }
    if (state[node] >= 1) {
        state[node] = -1;
        infected[node] = 0;
    }
    return (0);
}

/**
 * Removes immunity from passed node. Checks if the node dead, or if the node has immunity to lose before updating. Variant history, immunity string and state updated to reflect new information.
 *
 * @param node  Index representing node to be updated.
 * @return          If the program completes successfully, returns 0.
 */
int Graph::removeImmunity(int node) {
    int variant = 0;
    if (node < 0 | node >= nn) {
        cout << "Error. Node outside Bounds.\n";
        return (2);
    }
    if ((state[node] < 1) && (state[node] != -2)) //(maybe remove this?)
    {
        state[node] = 0; // TODO: Double check if this is accurate/needed (what does -1 mean).
        if (variantHistory[node].size() > 0) {
            variant = variantHistory[node][0] - 1;
            for (int x = 0; x < immunityDic[node].size(); x++) {
                immunityDic[node][x] = immunityDic[node][x] - VariantDic[variant][x];
            }
            variantHistory[node].erase(variantHistory[node].begin());
        }

    }
    return (0);
}

/**
 * Additional calculation to include immunity string into probability of infection.
 *
 * @param Ac  Integer vector representing the number of neighbours each node has that is infected with the variant passed.
 * @param x  index of node involved in the calculation.
 * @param totalBits  length of variant bitstring.
 * @param variant  index of variant in VariantDic.
 * @return          Returns the total number of mismatches between the immunity string and the variant string.
 */
double Graph::calcExtra(vector<int> Ac, int x, int totalBits, int variant) {
    if (Ac[x] == 0) {
        return (0.0);
    }
    int mismatches = 0;
    if (Ac[x] > 0) {
        int dif = 0;
        for (int y = 0; y < VariantDic[variant].size(); y++) {
            dif = VariantDic[variant][y] - immunityDic[x][y];
            if (dif >= 1) {
                mismatches++;
            }
        }
    }
    return ((double) mismatches / totalBits);
}

/**
 * Calculates the probability of infection of all nodes given a variant. Ask for more details on calculation.
 *
 * @param Ac  Integer vector representing the number of neighbours each node has that are infected with with the given variant.
 * @param variant  index of variant in VariantDic.
 * @return         Returns the probability of infection for all nodes, and returned as a single vector.
 */
vector<double> Graph::infectionProb(vector<int> Ac, int variant) {
    vector<double> probs;
    vector<double> extra;
    vector<double> extra2;
    int totalBits = 0;
    for (int x = 0; x < VariantDic[variant].size(); x++) {
        totalBits = totalBits + VariantDic[variant][x];
    }

    double a = 0.0;
    for (int x = 0; x < Ac.size(); x++) {
        a = calcExtra(Ac, x, totalBits, variant);
        extra.push_back(a);
    }
    double b = 1.0;
    if(alpha == 1.0)
    {
        for (int x = 0; x < Ac.size(); x++) {
            probs.push_back(Ac[x]);
        }
        return(probs);
    }

    b = log(1 - alpha);

    for (int x = 0; x < Ac.size(); x++) {
        extra2.push_back(Ac[x] * b);
    }

    for (int x = 0; x < Ac.size(); x++) {
        extra2[x] = 1 - exp(extra2[x]);
    }

    for (int x = 0; x < Ac.size(); x++) {
        probs.push_back(extra2[x] * extra[x]);
    }

    return (probs);
}

/**
 * Calculates the number of infected neighbours for each node for a given variant.
 *
 * @param variant  Integer vector representing the immunity string for the new variant.
 * @return         A vector of integers representing the number of neighbouring nodes that are infected with the variant passed.
 */
vector<int> Graph::calcAc(int variant) {
    vector<int> Ac;
    vector<int> mask;
    for (int x = 0; x < state.size(); x++) {
        if (state[x] == variant + 1) {
            mask.push_back(1);
        } else {
            mask.push_back(0);
        }
    }
    int a;
    for (int x = 0; x < nn; x++) {
        a = 0;
        for (int y = 0; y < nn; y++) {
            a = a + (adj[x][y] * mask[y]);
        }
        Ac.push_back(a);
    }
    return (Ac);
}

/**
 * Given a vector representing the next nodes to be infected, infect each one with the given variant.
 *
 * @param new1    Vector, where the index of the vector that has a value 1 indicates the index of the node that is to be infected, and 0 indicates the index of the node that is not.
 * @param variant   index of variant in VariantDic.
 * @return          If the program completes successfully, returns 0.
 */
int Graph::runInfections(vector<int> new1, int variant) {
    for (int x = 0; x < new1.size(); x++) {
        if ((state[x] == 0 || state[x] == -1) && new1[x] == 1) // TODO: Check this if error happens
        {
            if (infect(x, variant) != 0) {
                cout << "ERROR! Infection did not go through!" << endl;
            }
        }
    }
    return (0);
}

/**
 * Checks each node to see if it recovers from infection during this timestep and uninfects them. The chance of recovery is defined in recProb.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::recoveries() {
    double randVar = 0;
    for (int x = 0; x < state.size(); x++) {
        if (state[x] >= 1) {
            randVar = (rand() % 100) / 100.0;
            if (randVar < recProb) {
                unInfect(x);
            }
        }
    }
    return (0);
}

/**
 * Changes the passed node to the dead state. Ensures the node exists (>0, < total number of nodes). Ensures the node is infected (state >= 1), just so it's
 * not an arbitrary death. Updates internal datastructures to reflect the new information.
 *
 * @param node  index of the variant to be put to death. (LOL)
 * @return          If the program completes successfully, returns 0.
 */
int Graph::death(int node) {
    if ((node < 0) || (node >= nn)) {
        cout << "Error. Node outside Bounds.\n";
        return (2);
    }
    if (state[node] >= 1) {
        state[node] = -2;
        infected[node] = 0;
        deathcount++;
    }
    return (0);
}

/**
 * Checks each node to see if it loses immunity to the oldest variant it still has immunity to during this timestep and removes immunity from them. The
 * chance of immunity decay is defined in decProb. The node has have been infected previously at least once (have a variant history vector size > 0), and
 * must be recovered from infection but not dead.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::ImmunityDecay() {
    double rand1 = 0.0;
    for (int x = 0; x < nn; x++) {
        if ((!variantHistory[x].empty()) && (state[x] != -2)) {
            if (variantHistory[x][0] != state[x]) {
                rand1 = (rand() % 100) / 100.0;
                if (rand1 < decProb) {
                    removeImmunity(x);
                }
            }
        }
//        {
//            rand1 = (rand() % 100) / 100.0;
//            if (rand1 < decProb) {
//                removeImmunity(x);
//            }
//        }
    }
    return (0);
}

/**
 * Given a variant, calculate the infection probability for each node, and return a vector indicating which nodes will be infected with that variant this timestep.
 *
 * @param variant   index of variant in VariantDic.
 * @return          Vector of integers representing which nodes are to be infected with the passed variant during the current timestep.
 */
vector<int> Graph::runVariant(int variant) {
    vector<int> newInfections;
    double rands = 0.0;
    vector<int> Ac = calcAc(variant);
    vector<double> infectProb = infectionProb(Ac, variant);
    for (int x = 0; x < nn; x++) {
        rands = (rand() % 100) / 100.0;
        if (rands < infectProb[x]) {
            newInfections.push_back(1);
        } else {
            newInfections.push_back(0);
        }
    }
    return (newInfections);
}

/**
 * Runs the next timestep. In this function we: calculate the next to be infected for each variant, check who loses immunty and remove it, check who recovers
 * and change state, check who dies and kill them, then infect anyone who needs to be infected. Currently we infect in variant dictionary order.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::updateInfected() {
    vector<vector<int> > infectionListList;
    vector<vector<int> > holder;
    for (int x = 0; x < VariantDic.size(); x++) {
        holder.push_back(runVariant(x));
    }

    ImmunityDecay();
    recoveries();
    kill();

    for (int x = 0; x < VariantDic.size(); x++) {
        runInfections(holder[x], x);
    }

    return (0);
}

/**
 * Prints the Adjacancy matrix to the commandline. Mostly for debgging purposes.
 *
 * @return
 */
void Graph::printAdj() {
    for (int x = 0; x < adj.size(); x++) {
        for (int y = 0; y < adj[x].size(); y++) {
            cout << adj[x][y] << " ";
        }
        cout << "\n";
    }
}

/**
 * Adds a new variant to the simulation and infects somebody who is not currently infected.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @return          If the program completes successfully, returns 0. If the new variant failed to infect someone, returns 1.
 */
int Graph::newVariant(vector<int> bits) {
    // TODO: Possibly look into different variant intro?
    int counter = 0;
    vector<int> holder;
    int worked = 1;
    for (int x = 0; x < immunitySize; x++) {
        holder.push_back(bits[x]);
    }
    VariantDic.push_back(holder);
    int node = rand() % nn;
    worked = infect(node, VariantDic.size() - 1);
    while (worked == 1) {
        node = rand() % nn;
        worked = infect(node, VariantDic.size() - 1);
        counter++;
        if (counter > nn) {
            cout << "New Variant failed to infect.\n";
            return (1);
        }
    }
    return (0);
}

/**
 * Checks which nodes are dying this timestep assuming the node is currently alive (on the outside), and kills them.
 *
 * @return
 */
void Graph::kill() {
    double rands = 0.0;
    for (int x = 0; x < nn; x++) {
        if (state[x] > 0) {
            rands = (rand() % 100) / 100.0;
            if (rands < deathProb) {
                death(x);
            }
        }
    }

//    for (int x = 0; x < nn; x++) {
//        if (state[x] > 0) {
//            death(x);
//        }
//    }
}
