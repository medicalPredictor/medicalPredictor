#include "Epidemic.h"

/**
 * TODO: Fill in.
 * @param network
 * @param alpha
 * @param immuneSize
 * @param initVariant
 * @param probIS
 * @param probXS
 * @param probIR
 * @return
 */
int Epidemic::initialize(Graph network, double alpha, int immuneSize, vector<bool> initVariant, double probIS, double probXS, double probIR) {
    this->network = network;
    this->alpha = alpha;
    this->immuneSize = immuneSize;
    variantDic.push_back(initVariant);
    this->probIS = probIS;
    this->probXS = probXS;
    this->probIR = probIR;
    return (0);
}
/**
 *
 * @param dogs
 * @return
 */
int Epidemic::thing(int dogs){

}