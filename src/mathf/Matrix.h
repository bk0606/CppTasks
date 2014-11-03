#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include <bits/stl_algobase.h>
#include "../utils/Console.h"
#include "Mathf.h"

using namespace utils;

namespace mathf {

    class Matrix {
    public:
        unsigned rowsCnt, colsCnt, elemsCnt;

        static
        Matrix      transpose(const Matrix& matrix);
        static
        Matrix      getDiagonal(const Matrix& matrix);

        void        assignData(int* dataArray, unsigned rowsCount, unsigned colsCount);
        int*        begin() const;
        int*        end() const;
        void        printToConsole(char const* title);

        Matrix&     operator= (const Matrix& rightMtx);
        Matrix      operator+ (const Matrix& withMtx) const;
        Matrix      operator* (const Matrix& toMtx) const;
        int&        operator() (unsigned rowIndex, unsigned colIndex);
        int         operator() (unsigned rowIndex, unsigned colIndex) const;

                    Matrix(unsigned rowsCnt, unsigned colsCnt);
                    Matrix(int* dataArray, unsigned rowsCnt, unsigned colsCnt);
                    Matrix(const Matrix& copyingMtx);
                    ~Matrix();

    private:
        int* _data;
    };

}

#endif


