#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;

namespace utils {

    class Console {
    public:
        static void printArray(int *array, unsigned size);
        static void printArray(char const *title, int *array, unsigned size);

        static void printVector(vector<int> const &vect);

        static void printMatrix(vector<vector<int> > const &matrix);
        static void printMatrix(int *const matrix, unsigned rows, unsigned cols);

    };

}

#endif
