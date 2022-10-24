#include "Graph.h"

/**
 * Scales our data to realworld data
 * INPUT:
 * intData -> data from the M.P
 * timeScale -> averaging done by real world data at each timestep
 * typeAgg -> type of averaging: 0 == left weighted, 1 == centered weighted, 2 == right weighted
 * OUTPUT: 
 * aggData -> data averaged based upon real world timescale
 */
float ** aggregateData(vector<float> intData, vector<float> timeScale, int typeAgg) {
    vector<float> aggData{intData};
    for (int i=0; i<size(aggData); i++){
        aggData[i] = 0
        for( int j=0; j<timeScale[i]; j++){
            if typeAgg == 0 {
            // Force it to spit out 0 if not enough data points
                if i-j <0{
                    sum = 0
                    break;
                } else {
                    sum += intData[i-j];
                }
            }
        }
    }

    return (0);
}

/**
* Takes real data and simulated data, outputs difference in points squared
* INPUT:
* realData -> data from real world location
* simData -> data made from M.P
* OUTPUT:
* variance -> summed difference squared
 */
int vars(vector<float> realData, vector<float> simData) {
    variance = 0;
    // calculates variance :^)
    return (variance);
}