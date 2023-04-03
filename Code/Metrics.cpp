#include <cmath>
#include <vector>
#include <iostream>
#include <tuple>

using namespace std;

/**
 * Takes real data and simulated data, outputs variance (RMS)
 *
 * @param realData -> data from real world location
 * @param simData -> data made from M.P
 * @return variance -> RMS variance
 */
double vars(vector<float> realData, vector<float> simData) {
    double variance = 0;
    // calculates variance :^)
    if (realData.size() != simData.size()) {
        cout << "ERROR in method 'vars': realData and simData different sizes." << endl;
        return (-1);
    }

    for (int i = 0; i < realData.size(); ++i) {
        variance += pow(realData.at(i) - simData.at(i), 2);
    }

    variance /= (double) realData.size();
    variance = sqrt(variance);

    return variance;
}

/**
 * This function takes in a vector of original data and aggregates the data into bins of size binSize.  The first bin has start index S (inclusive) in the
 * interval [0, binSize) end index S + binSize (exclusive) and each successive binSize number of data points is another bin.  We return a vector of floats
 * for each potential start index representing the sum of the data points contained in each bin.  If keepEnds is true the data points before the first bin
 * are aggregated and appear as the first float in the vector.  Similarly, this is done for the data points after the last bin.
 *
 * @param ogData
 * @param binSize
 * @param keepEnds
 * @return
 */
vector<vector<float> > bsAggr(vector<float> ogData, int binSize, bool keepEnds) {
    vector<vector<float>> results;
    vector<float> bins;
    float binSum = 0.0;
    int numBins;

    if (ogData.size() < binSize) {
        cout << "ERROR in method 'aggr': binSize larger than ogData size." << endl;
        return results;
    }

    for (int bs = 0; bs < binSize; ++bs) {
        numBins = (int) (ogData.size() - bs) / binSize;
        binSum = 0;
        if (keepEnds && bs != 0) {
            for (int d = 0; d < bs; ++d) {
                binSum += ogData[d];
            }
            bins.push_back(binSum);
        }
        for (int b = 0; b < numBins; ++b) {
            binSum = 0.0;
            for (int d = bs + b * binSize; d < bs + (b + 1) * binSize; ++d) {
                binSum += ogData[d];
            }
            bins.push_back(binSum);
        }
        if (keepEnds && (ogData.size() - bs) % binSize != 0) {
            binSum = 0;
            for (int i = bs + numBins * binSize; i < ogData.size(); ++i) {
                binSum += ogData[i];
            }
            bins.push_back(binSum);
        }
        results.push_back(bins);
        bins.clear();
    }
    return results;
}

/**
 * This function takes in a vector of floats ogData and bins the data points according to the time dimension data contained as 3-tuples in tdims.  Each tuple
 * has three values representing: bin size, start index, and end index to be used when binning.
 *
 * @param tdims
 * @param ogData
 * @return
 */
vector<float> aggregator(const vector<tuple<int, int, int> > &tdims, vector<float> ogData) {
    vector<float> results;

    for (tuple<int, int, int > tup: tdims){
        auto start = ogData.begin() + get<1>(tup);
        auto end = ogData.begin() + get<2>(tup) + 1;

        vector<float> subVec(start, end);
        vector<float> sprime = bsAggr(subVec, get<0>(tup), false).front(); // TODO: deal with multiple vectors

        results.insert(results.end(), sprime.begin(), sprime.end());
    }

    return results;
    //    for (tuple<int, int, int> t: tdims) {
    //        cout << get<0>(t) << '\t' << get<1>(t) << '\t' << get<2>(t) << endl;
    //    }
}

/**
 * This function takes in a vector of floats representing epidemic data and locates all the likely bin sizes assuming ...
 *
 * @return
 */
int main() {
    vector<float> v1 = {1, 2, 3};
    vector<float> v2 = {3, 4, 7};
    cout << vars(v1, v2) << endl;

    vector<float> v3 = {20, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4};
    vector<vector<float> > lester = bsAggr(v3, 5, true);

    for (int i = 0; i < lester.size(); ++i) {
        for (int j = 0; j < lester[i].size(); ++j) {
            cout << lester[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    vector<float> v4;
    v4.reserve(100);
    for (int i = 0; i < 100; ++i) {
        v4.push_back(1);
    }

    vector<tuple<int, int, int> > tdims;
    tuple<int, int, int> t1(1, 0, 2);
    tuple<int, int, int> t2(2, 3, 8);
    tuple<int, int, int> t3(3, 9, 14);
    tuple<int, int, int> t4(5, 15, 29);
    tuple<int, int, int> t5(10, 30, 99);
    tdims.push_back(t1);
    tdims.push_back(t2);
    tdims.push_back(t3);
    tdims.push_back(t4);
    tdims.push_back(t5);

    vector<float> violet = aggregator(tdims, v4);

    for (int i = 0; i < violet.size(); ++i) {
        cout << violet[i] << " ";
    }
    cout << endl;
}