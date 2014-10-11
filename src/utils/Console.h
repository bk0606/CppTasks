#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>

using namespace std;

namespace utils {

    class Console {
    public:
        static void printVector(vector<int> const &vect);

        static void printMatrix(vector<vector<int> > const &matrix);
    };

}

#endif
