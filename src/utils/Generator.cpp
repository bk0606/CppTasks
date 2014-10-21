#include "Generator.h"

namespace utils {

    int* Generator::generateArray(int size, int dispersion) {
        int* array = new int[size];
        for (int i = 0; i < size; ++i) {
            array[i] = rand() % dispersion;
        }
        return array;
    }

    vector<int> Generator::generateVector(int size, int dispersion) {
        vector<int> vect;
        for (int i = 0; i < size; ++i) {
            vect.push_back(rand() % dispersion);
        }
        return vect;
    }

    vector<vector<int> > Generator::generateMatrixVector(int rows, int cols, int dispersion) {
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