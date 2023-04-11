#include <iostream>
#include "CovidModeler.h"
#include "Graph.h"

int main() {
    srand48((int) time(nullptr));
    srand((int) time(nullptr));
    vector<vector<int> > All_the_simulations;
    int sims = 1000;
    for (int y = 0; y < sims; y++) {
        All_the_simulations.push_back(simulation());
    }
    //All_the_simulations.push_back(simulation());
    vector<double> results;
    for (int x = 0; x < 256; x++) {
        results.push_back(0.0);
    }

    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < sims; y++) {
            if (All_the_simulations[y].size() > x) {
                results[x] = results[x] + All_the_simulations[y][x];
            }
        }
    }
    for (int x = 0; x < 256; x++) {
        results[x] = double(results[x]) / sims;
    }

    ofstream vals;
    vals.open("./CovidModellerResults.txt", ios::out);
    for (auto &val: results) {
        if (val > 0) {
            cout << val << endl;
            vals << val << endl;
        }
    }
    vals.close();
}

/**
 * Debugging Tests.
 *
 * @return          If the program completes successfully, returns 0.
 */
int tests() {
    srand((int) time(NULL));
    Graph a;
    int bits1[] =
        {0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0,
         1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1,
         0, 0, 1, 0, 1, 0};
    vector<int> bits;
    std::copy(std::begin(bits1), std::end(bits1), std::back_inserter(bits));

    cout << "bitstring length: " << bits.size() << "\n";
    a.initialize(10, bits, 4, bits, 0.63, 0.125514, 0.00666641798664, 0.00168214);
    //a.initialize(5, bits, 4, bits, 0.63, 0.125514, 0.5);
    a.infect(0, 0);
    vector<int> cState = a.getState();
    printVector(cState);

    a.infect(1, 0);
    cState = a.getState();
    printVector(cState);

    a.infect(4, 0);
    cState = a.getState();
    printVector(cState);

    a.unInfect(0);
    cState = a.getState();
    printVector(cState);

    cout << "removing immnity\n";

    a.removeImmunity(0);
    cState = a.getState();
    printVector(cState);

    vector<int> Ac = a.calcAc(0);
    printVector(Ac);
    vector<double> probs = a.infectionProb(Ac, 0);
    printVector(probs);

    vector<int> newInfections;

    for (int x = 0; x < probs.size(); x++) {
        if (probs[x] > 0) {
            newInfections.push_back(1);
        } else {
            newInfections.push_back(0);
        }
    }
    cout << "NewInfections: ";
    printVector(newInfections);

    a.runInfections(newInfections, 0);
    cState = a.getState();
    printVector(cState);

    a.recoveries();
    cState = a.getState();
    printVector(cState);

    cout << "killing 4\n";
    a.death(4);
    cState = a.getState();
    printVector(cState);

    a.unInfect(1);
    cState = a.getState();
    printVector(cState);

    a.ImmunityDecay();
    cState = a.getState();
    printVector(cState);

    a.infect(2, 0);
    cState = a.getState();
    printVector(cState);

    newInfections.clear();

    newInfections = a.runVariant(0);
    cState = a.getState();
    printVector(cState);

    a.runInfections(newInfections, 0);
    cState = a.getState();
    printVector(cState);

    a.unInfect(0);
    cState = a.getState();
    printVector(cState);

    a.removeImmunity(0);
    cState = a.getState();
    printVector(cState);

    a.updateInfected();
    cState = a.getState();
    printVector(cState);

    a.printAdj();

    int bits2[] = {1, 0, 0, 1, 1, 1};
    std::copy(std::begin(bits2), std::end(bits2), std::back_inserter(bits));

    a.newVariant(bits);
    cState = a.getState();
    printVector(cState);

    return (0);
}

/**
 * Given given a bitstring, start at a random location in the string, and extract a bitstring of length length to use as the next immunity string for a new variant.
 *
 * @param bits  Integer vector of zeros and ones. Can be of any length greater than varianble length.
 * @param length  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector<int> makeNewVariant(vector<int> bits, int length) {
    int rands = rand() % (bits.size() - length);
    //int rands = 1;
    vector<int> output;
    for (int x = rands; x < rands + length; x++) {
        output.push_back(bits[x]);
    }
    return (output);
}

/**
 * Main used to testing purposes. Can be removed later.
 *
 * @return          If the program completes successfully, returns 0.
 */
vector<int> simulation() {
    //vector< vector < int > All_the_simulations;

    //tests();
    int newVariantsFlag = 2;
//    srand48((int) time(NULL));
//	srand((int) time(NULL));
    std::vector<int> listOfPoints;
    ReadData("../bs_test.txt", &listOfPoints, 100000);

    //cout << "Here\n";
//    printVector(listOfPoints);

    vector<int> infectedLog;
    vector<int> deathLog;
    vector<int> lifeLog;
    vector<int> TotalNewInfectLog;
    int immunityLength = 7;

    //cout << "HERE1" << endl;

    Graph a;
    //a.initialize(256, listOfPoints, immunityLength, listOfPoints, 0.63, 0.126, 0.006666, 0.00168214);
    a.initialize(256, listOfPoints, immunityLength, listOfPoints, 0.5, 0.0, 0.0, 1.0);
    //cout << "HERE1.105" << endl;
    //a.printAdj();
    a.infect(0, 0);
    vector<int> infected;
    vector<int> states;
    infected = a.getInfected();
    //printVector(infected);
    infectedLog.push_back(1);
    deathLog.push_back(0);
    lifeLog.push_back(256);
    TotalNewInfectLog.push_back(1);
    //cout << 1 << endl;

    int max_timeSteps = 1000;
    int count = 0;
    double variantProb = 0.01;
	int newInfectedDate = 5;

    //cout << "HERE1.1" << endl;

    vector<int> stats;
    stats = a.nextTimeStep();
    int sum = 0;
    for (auto &val: infectedLog) {
        sum += val;
    }
//    printVector(stats);
//    cout << "HERE1.15" << endl;
    //cout << stats[0] << endl;
//    cout << "HERE1.17" << endl;
    infectedLog.push_back(stats[0]);
    deathLog.push_back(stats[1]);
    lifeLog.push_back(stats[2]);
    sum = 0;
    for (int x = 3; x < stats.size(); x++) {
        sum = sum + stats[x];
    }
    TotalNewInfectLog.push_back(sum);
    double variantTest = 0.0;

//    cout << "HERE2" << endl;

    std::vector<int> variant2;

    while ((stats[0] > 0) && (stats[0] < 256) && (count < max_timeSteps)) {
        if (newVariantsFlag == 1) {
            variantTest = drand48();
            if (variantTest < variantProb) {
                cout << "Adding New Variant\n";
                variant2 = makeNewVariant(listOfPoints, immunityLength);
                a.newVariant(variant2);
            }
        }
        if (newVariantsFlag == 2) {
            if (count == newInfectedDate) {
                //cout << "Adding New Variant\n";
                variant2 = makeNewVariant(listOfPoints, immunityLength);
                a.newVariant(variant2);
            }
        }
        stats = a.nextTimeStep();
		//cout << "Here\n";
		//printVector(stats);
        sum = 0;
        for (auto &val: infectedLog) {
            sum += val;
        }
        //cout << stats[0] << endl;
        states = a.getState();
        //cout << "\n";
        //printVector(states);
//        stats = a.nextTimeStep();

        infectedLog.push_back(stats[0]);
        //printVector(infectedLog);
        deathLog.push_back(stats[1]);
        lifeLog.push_back(stats[2]);
        // sum = 0;
//         for (int x = 3; x < stats.size(); x++) {
//             sum = sum + stats[x];
//         }
//         TotalNewInfectLog.push_back(sum);
		TotalNewInfectLog.push_back(stats[0]);
		//cout << sum << "\n";

        count++;
		//cout << count << "\n";
    }
	
    int final1 = a.numOfVariants();
    // printVector(infectedLog);
    //printVector(deathLog);
    // printVector(lifeLog);
	//cout << final1 << "\n";
    //writeToFile(infectedLog, "InfectedLog.txt");
    //writeToFile(deathLog, "DeathLog.txt");
    //writeToFile(lifeLog, "LifeLog.txt");
    return (TotalNewInfectLog);
}

/**
 * Reads in a txt file and saves to the vector of ints passed to the function.
 *
 * @param input1  name of file to be imported.
 * @param listOfPoints  pointer to a vector of integers to be populated with the information from the input.
 * @param linesize  Number of characters expected in each line of the input file.
 * @return        
 */
void ReadData(string input1, std::vector<int> *listOfPoints, int linesize) {//read in the data

    fstream input;
    char buf1[linesize];
    string buf2;
    string line;

    string inputFile = input1;

    std::vector<int> out;
    input.open(inputFile, ios::in);
    input.getline(buf1, linesize); //Get first line
    line = buf1;
    tokenize(line, '\t', &out);
    (*listOfPoints) = out;
    input.close();
}

/**
 * Takes a string of characters and delimits them by the passed delimiter. 
 *
 * @param str  string to be delimited.
 * @param delim  character representing the delimiter to be used to break up the string.
 * @param out pointer to a vector of ints that will be populated by the delimited string information.
 * @return      
 */
void tokenize(std::string const str, const char delim, std::vector<int> *out) {
    std::stringstream ss(str);
    std::string s;
    std::vector<string> temp;
    while (std::getline(ss, s, delim)) {
        temp.push_back(s);
    }

//    std::vector<string>::iterator it;
    for (int i = 0; i < temp.size(); ++i) {
        int num = atoi(temp.at(i).c_str());
        out->push_back(num);
    }
}

/**
 * Given a vector of ints, and a filename, write the values in the vector to the file, one int per line.
 *
 * @param a  vector of integers to be written to file.
 * @param filename  string representing the file to be written to. Anything in the file before will be overwritten.
 * @return    
 */
void writeToFile(vector<int> a, string filename) {
    fstream aus;  //output file
    aus.open(filename, ios::out);
    aus.precision(10);

    for (int i = 0; i < a.size(); i++) {
        aus << a[i] << "\n";
    }
    aus.close();
}

// TODO: Defensive library.
