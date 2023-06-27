#include <iostream>
#include "CovidModeler.h"
#include "Graph.h"


//int main() {
//    srand48((int) time(nullptr));
//    srand((int) time(nullptr));
//    vector<vector<int> > All_the_simulations;
//	std::vector < vector<int> > variantList;
//    int sims = 30;
//	int variants = 0;
//	vector < int > newInfectedDate;
//	newInfectedDate.push_back(5);
//	newInfectedDate.push_back(10);
//	newInfectedDate.push_back(15);
//	string Outputfile = "./DATA.DAT";
//	ReadData2("../variants.txt", &variantList, 100000);
//	std::vector<int> listOfPoints;
//    ReadData("../bs_test.txt", &listOfPoints, 100000);
//	ReadData2("../variants.txt", &variantList, 100000);
//	vector<double> results;
//	results = runRun(Outputfile, sims, 256, 7, 0.63, 0.126, 0.006666, 0.00168214, 0.01, listOfPoints, variantList, newInfectedDate, false);
//}

vector<double> runRun(string Outputfile, int sims, int nn, int variantSize, double alpha, double recov, double decay, double death, double variantProb, std::vector<int> listOfPoints, std::vector < vector<int> > variantList, vector < int > newInfectedDate, bool FileFlag)
{
	ofstream vals;
	int variants = 0;
	vector<vector<int> > All_the_simulations;
	vector < vector < int > > TotalNewVariantInfectedLog;
	//string Outputfile = "./DATA.DAT";
	if(FileFlag)
	{
	    vals.open(Outputfile, ios::out);
		vals << "Experiment 1" << endl;
		vals << "__" << endl;
	    vals.close();
	}
	Graph a;
	
    for (int y = 0; y < sims; y++) {
		//a = new Graph;
		a.initialize(nn, listOfPoints, variantSize, variantList[0], alpha, recov, decay, death, variantProb);
        All_the_simulations.push_back(a.simulation(Outputfile, variantList, &variants, newInfectedDate, &TotalNewVariantInfectedLog, FileFlag));
		//delete a;
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
	return(results);
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

    string inputFile = input1;

    std::vector<int> out;
    input.open(inputFile, ios::in);
	
	while(input.getline(buf1, linesize))
	{
		line = buf1;
	    tokenize(line, '\t', &out);
	    (*listOfPoints).push_back(out);
		for(int x = out.size(); x > 0; x--)
		{
			out.pop_back();
		}
	}
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
