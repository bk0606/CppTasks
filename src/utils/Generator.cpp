#include "Generator.h"
#include <stdlib.h>

using namespace std;

namespace utils {

    vector<int> Generator::generateVector(int size, int dispersion) {
        vector<int> vect;
        for (int i = 0; i < size; ++i) {
            vect.push_back(rand() % dispersion);
        }
        return vect;
    }

    vector<vector<int> > Generator::generateMatrix(int rows, int cols, int dispersion) {
        vector<vector<int> > vect;
        for (int i = 0; i < rows; ++i) {
            vector<int> tmpVect;
            for (int j = 0; j < cols; ++j) {
                tmpVect.push_back(rand() % dispersion);
            }
            vect.push_back(tmpVect);
        }
        return vect;
    }

}