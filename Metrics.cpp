#include <cmath>
#include <vector>
#include <iostream>

#define verbose

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
    if (realData.size() != simData.size()){
        cout<<"ERROR in method 'vars': realData and simData different sizes."<<endl;
        return (-1);
    }

    for (int i = 0; i < realData.size(); ++i) {
        variance += pow(realData.at(i)-simData.at(i),2);
    }

    variance /= (double)realData.size();
    variance = sqrt(variance);

    return variance;
}

vector<vector<float > > aggr(vector<float> ogData, int binSize, bool keepEnds){
    vector<vector<float>> results;
    vector<float> bins;
    float binSum = 0.0;
    int numBins;

    if (ogData.size() < binSize){
        cout<<"ERROR in method 'aggr': binSize larger than ogData size."<<endl;
        return results;
    }

    for (int bs = 0; bs < binSize; ++bs) {
        numBins = (int) (ogData.size() - bs) / binSize;
        binSum=0;
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

int main(){
    vector<float> v1 = {1,2,3};
    vector<float> v2 = {3,4,7};
    cout<<vars(v1,v2)<<endl;

    vector<float> v3 = {20, 1, 1, 1, 2, 2, 2, 3, 3, 3,4,4};
    vector<vector<float > > lester = aggr(v3, 20, true);

    for (int i = 0; i < lester.size(); ++i) {
        for (int j = 0; j < lester[i].size(); ++j) {
            cout<<lester[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}