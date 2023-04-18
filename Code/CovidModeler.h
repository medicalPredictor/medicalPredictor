#include <cmath>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>

using namespace std;

void ReadData(string input1, std::vector<int> *listOfPoints, int linesize);
void tokenize(std::string const str, const char delim, std::vector<int> *out);
void writeToFile(vector<int> data, string filename);
vector<int> simulation(string outfile);
int printVector(vector<int> vec);
void ReadData2(string input1, std::vector <std::vector<int> > *listOfPoints, int linesize);

/**
 * Prints an integer vector. Mostly for debugging purposes.
 *
 * @param vec  A vector of intergers of any length.
 * @return          If the program completes successfully, returns 0.
 */
int printVector(vector<int> vec) {
    for (int y = 0; y < vec.size(); y++) {
        cout << vec[y] << " ";
    }
    cout << "\n";
    return (0);
}

/**
 * Prints a vector of doubles. Mostly for debugging purposes.
 *
 * @param vec  A vector of doubles of any length.
 * @return          If the program completes successfully, returns 0.
 */
int printVector(vector<double> vec) {
    for (int y = 0; y < vec.size(); y++) {
        cout << vec[y] << " ";
    }
    cout << "\n";
    return (0);
}