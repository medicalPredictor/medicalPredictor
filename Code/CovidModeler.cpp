#include <iostream>
#include "CovidModeler.h"
#include "Graph.h"

int main() {
	ofstream vals;
    srand48((int) time(nullptr));
    srand((int) time(nullptr));
    vector<vector<int> > All_the_simulations;
	std::vector < vector<int> > variantList;
    int sims = 2;
	int variants = 0;
	vector < int > newInfectedDate;
	newInfectedDate.push_back(5);
	newInfectedDate.push_back(10);
	newInfectedDate.push_back(15);
	string Outputfile = "./DATA.DAT";
    vals.open(Outputfile, ios::out);
	vals << "Experiment 1" << endl;
	vals << "__" << endl;
    vals.close();
	ReadData2("../variants.txt", &variantList, 100000);
	vector < vector < int > > TotalNewVariantInfectedLog;
    for (int y = 0; y < sims; y++) {
        All_the_simulations.push_back(simulation(Outputfile, variantList, &variants, newInfectedDate, &TotalNewVariantInfectedLog));
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

    //ofstream vals;
    vals.open("./CovidModellerResults.txt", ios::out);
    for (auto &val: results) {
        if (val > 0) {
            //cout << val << endl;
            vals << val << endl;
        }
    }
    vals.close();
	
	//cout << variants << "\n";
	// vals.open(Outputfile, ios::out|ios::app);
// 		//printVector(variantList[0]);
// 	cout << "*577+-" << endl;
// 	vals << "*577+-" << endl;
// 		for(int x = 0; x < variants; x++)
// 		{
// 			printVector(variantList[x]);
// 			printVector(variantList[x], vals);
//
// 			if(x== 0)
// 			{
// 				cout << "Start Date: " << 0 << "\n";
// 				vals << "Start Date: " << 0 << "\n";
// 			}
// 			if(x > 0)
// 			{
// 				cout << "Start Date: " << newInfectedDate[x-1] << "\n";
// 				vals << "Start Date: " << newInfectedDate[x-1] << "\n";
// 			}
// 			if(x > 0)
// 			{
// 				for(int q = 0; q < newInfectedDate[x-1]; q++)
// 				{
// 					cout << 0 << " ";
// 					vals << 0 << " ";
// 				}
// 			}
//
// 			for(int y = 0; y < TotalNewVariantInfectedLog.size(); y++)
// 			{
// 				if(x == 0)
// 				{
// 					cout << TotalNewVariantInfectedLog[y][x] << " ";
// 					vals << TotalNewVariantInfectedLog[y][x] << " ";
// 				}
// 				if(x > 0)
// 				{
// 					if(x < TotalNewVariantInfectedLog[y].size())
// 					{
// 						cout << TotalNewVariantInfectedLog[y][x] << " ";
// 						vals << TotalNewVariantInfectedLog[y][x] << " ";
// 					}
// 				}
//
// 			}
// 			cout << "\n";
// 			vals << "\n";
// 		}
// 	vals.close();
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
vector<int> simulation(string outfile, std::vector < vector<int> > variantList, int * final1, vector < int > newInfectedDate, vector < vector < int > > * TotalNewVariantInfectedLog) {
    //vector< vector < int > All_the_simulations;

    //tests();
	//ofstream vals;
	//cout << (*TotalNewVariantInfectedLog).size() << "\n";
	while((*TotalNewVariantInfectedLog).size() > 0)
	{
		while((*TotalNewVariantInfectedLog)[(*TotalNewVariantInfectedLog).size()-1].size() > 0)
		{
			(*TotalNewVariantInfectedLog)[(*TotalNewVariantInfectedLog).size()-1].pop_back();
		}
		(*TotalNewVariantInfectedLog).pop_back();
	}
	//cout << (*TotalNewVariantInfectedLog).size() << "\n";
    int newVariantsFlag = 2;
//    srand48((int) time(NULL));
//	srand((int) time(NULL));
    std::vector<int> listOfPoints;
	//std::vector < vector<int> > variantList;
    ReadData("../bs_test.txt", &listOfPoints, 100000);
	ReadData2("../variants.txt", &variantList, 100000);
	

    vector<int> infectedLog;
    vector<int> deathLog;
    vector<int> lifeLog;
    vector<int> TotalNewInfectLog;
	vector<int> TotalNewDeathLog;
	vector<int> TotalNewRecovLog;
	//vector < vector < int > > TotalNewVariantInfectedLog;
    int immunityLength = 7;

    //cout << "HERE1" << endl;

    Graph a;
    a.initialize(256, listOfPoints, variantList[0].size(), variantList[0], 0.63, 0.126, 0.006666, 0.00168214);
    //a.initialize(256, listOfPoints, variantList[0].size(), variantList[0], 0.5, 0.0, 0.0, 1.0);
    //cout << "HERE1.105" << endl;
	int counter = 0;
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
	TotalNewDeathLog.push_back(0);
	TotalNewRecovLog.push_back(0);
    TotalNewInfectLog.push_back(a.newInfectionCount());
	TotalNewDeathLog.push_back(a.newDeathCount());
	TotalNewRecovLog.push_back(a.newRecoveriesCount());
	TotalNewVariantInfectedLog->push_back(a.getnewVariantInfectionsCount());
    //cout << 1 << endl;

    int max_timeSteps = 1000;
    int count = 0;
    double variantProb = 0.01;

    vector<int> stats;
    stats = a.nextTimeStep();
    int sum = 0;
    for (auto &val: infectedLog) {
        sum += val;
    }

    infectedLog.push_back(stats[0]);
    deathLog.push_back(stats[1]);
    lifeLog.push_back(stats[2]);

    TotalNewInfectLog.push_back(a.newInfectionCount());
	TotalNewDeathLog.push_back(a.newDeathCount());
	TotalNewRecovLog.push_back(a.newRecoveriesCount());
	TotalNewVariantInfectedLog->push_back(a.getnewVariantInfectionsCount());
    double variantTest = 0.0;

    std::vector<int> variant2;

    while ((stats[0] > 0) && (stats[0] < 256) && (count < max_timeSteps)) {
        if (newVariantsFlag == 1) {
            variantTest = drand48();
            if (variantTest < variantProb) {
                cout << "Adding New Variant\n";
				if(counter < variantList.size())
				{
					counter++;
	                a.newVariant(variantList[counter]);
				}
            }
        }
        if (newVariantsFlag == 2) {
			for(int x = 0; x < newInfectedDate.size(); x++)
			{
	            if (count == newInfectedDate[x]) {
	                //cout << "Adding New Variant\n";
	                //variant2 = makeNewVariant(listOfPoints, immunityLength);
					if(counter < variantList.size())
					{
						counter++;
		                a.newVariant(variantList[counter]);
					}
	            }
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
		TotalNewInfectLog.push_back(a.newInfectionCount());
		TotalNewDeathLog.push_back(a.newDeathCount());
		TotalNewRecovLog.push_back(a.newRecoveriesCount());
		TotalNewVariantInfectedLog->push_back(a.getnewVariantInfectionsCount());
		
		//cout << sum << "\n";

        count++;
		//cout << count << "\n";
    }
	
    *final1 = a.numOfVariants();
	//cout << "Here\n";
	ofstream vals;
    vals.open(outfile, ios::out|ios::app);
	if (vals.is_open())
	  {
		//cout << "HERe\n";

        cout << '1' << " ";
        vals << '1' << " ";
	    for (int x = 1; x < TotalNewInfectLog.size(); x++) {
	        if (infectedLog[x] > 0) {
	            cout << TotalNewInfectLog[x] << " ";
				vals << TotalNewInfectLog[x] << " ";
	            //vals << val << " ";
	        }
	    }
        cout << endl;
        vals << endl;
		
        cout << TotalNewDeathLog[0] << " ";
        vals << TotalNewDeathLog[0] << " ";
	    for (int x = 1; x < TotalNewDeathLog.size(); x++) {
	        cout << TotalNewDeathLog[x] << " ";
			vals << TotalNewDeathLog[x] << " ";
	    }
        cout << endl;
        vals << endl;	
		
        cout << TotalNewRecovLog[0] << " ";
        vals << TotalNewRecovLog[0] << " ";
	    for (int x = 1; x < TotalNewRecovLog.size(); x++) {
	        cout << TotalNewRecovLog[x] << " ";
			vals << TotalNewRecovLog[x] << " ";
	    }
        cout << endl;
        vals << endl;	
		
		cout << "___" << endl;
		vals << "___" << endl;
		
		
		//vals.open(Outputfile, ios::out|ios::app);
			//printVector(variantList[0]);
		//cout << "*577+-" << endl;
		//vals << "*577+-" << endl;
			for(int x = 0; x < *final1; x++)
			{
				printVector(variantList[x]);
				printVector(variantList[x], vals);

				if(x== 0)
				{
					cout << "Start Date: " << 0 << "\n";
					vals << "Start Date: " << 0 << "\n";
				}
				if(x > 0)
				{
					cout << "Start Date: " << newInfectedDate[x-1] << "\n";
					vals << "Start Date: " << newInfectedDate[x-1] << "\n";
				}
				if(x > 0)
				{
					for(int q = 0; q < newInfectedDate[x-1]; q++)
					{
						cout << 0 << " ";
						vals << 0 << " ";
					}
				}

				for(int y = 0; y < TotalNewVariantInfectedLog->size(); y++)
				{
					if(x == 0)
					{
						cout << (*TotalNewVariantInfectedLog)[y][x] << " ";
						vals << (*TotalNewVariantInfectedLog)[y][x] << " ";
					}
					if(x > 0)
					{
						if(x < (*TotalNewVariantInfectedLog)[y].size())
						{
							cout << (*TotalNewVariantInfectedLog)[y][x] << " ";
							vals << (*TotalNewVariantInfectedLog)[y][x] << " ";
						}
					}

				}
				cout << "\n";
				vals << "\n";
			}
			
		//cout << "*577+-" << endl;
		//vals << "*577+-" << endl;
		cout << "___" << endl;
		vals << "___" << endl;
		//vals.close();
		
	    vals.close();
	  }
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
 * Reads in a txt file and saves to the vector of ints passed to the function.
 *
 * @param input1  name of file to be imported.
 * @param listOfPoints  pointer to a vector of integers to be populated with the information from the input.
 * @param linesize  Number of characters expected in each line of the input file.
 * @return        
 */
void ReadData2(string input1, std::vector <std::vector<int> > *listOfPoints, int linesize) {//read in the data

    fstream input;
    char buf1[linesize];
    string buf2;
    string line;
	
	//cout << input1 << "\n";

    string inputFile = input1;
	
	//cout << inputFile << "\n";

    std::vector<int> out;
    input.open(inputFile, ios::in);
	
	//cout << inputFile << "\n";
    //input.getline(buf1, linesize); //Get first line
    //line = buf1;
	while(input.getline(buf1, linesize))
	{
		line = buf1;
		//cout << line << "\n";
		//cout << "\n";
		//cout << "Done line\n";
	    tokenize(line, '\t', &out);
		// for(int x = 0; x < out.size(); x++)
		// {
		// 	cout << out[x] << " ";
		// 	//out.pop_back();
		// }
		//cout << out << "\n";
		//cout << "\n";
		//cout << "Done line2\n";
	    (*listOfPoints).push_back(out);
		//cout << out.size() << "\n";
		for(int x = out.size(); x > 0; x--)
		{
			//cout << out[x] << " ";
			out.pop_back();
		}
		//cout << "\n";
	    //input.getline(buf1, linesize); //Get first line
	    //line = buf1;
	}
	
	//cout << inputFile << "\n";
    //tokenize(line, '\t', &out);
    //(*listOfPoints).push_back(out);
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
