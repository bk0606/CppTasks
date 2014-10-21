#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <vector>

using namespace std;

namespace utils {
    class Generator {
    public:
        static int* generateArray(int size, int dispersion);
        static vector<int> generateVector(int size, int dispersion);
        static vector<vector<int> > generateMatrixVector(int rows, int cols, int dispersion);
    };
}

#endif

