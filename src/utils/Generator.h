#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <vector>

using namespace std;

namespace utils {
    class Generator {
    public:
        static vector<int> generateVector(int size, int dispersion);
        static vector<vector<int> > generateMatrix(int rows, int cols, int dispersion);
    };
}

#endif

