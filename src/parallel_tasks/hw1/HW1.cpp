#include  <vector>
#include <iostream>
#include <omp.h>
#include "HW1.h"

using namespace std;

namespace parallel_tasks {

    vector<int> HW1::SimpleVectorsProduct(vector<int> &a, vector<int> &b) {
        unsigned long aSize = a.size();
        int i = 0;
        vector<int> c(aSize, 0);
        if (aSize != b.size()) {
            cout << "Error: vectors must have same size.";
            return c;
        }
#pragma omp parallel for private(i)
        for (i = 0; i < aSize; i++) {
            c[i] = a[i] * b[i]; // omp_get_thread_num();
        }
        return c;
    }

    int HW1::VectorsProductsSum(vector<int> &a, vector<int> &b) {
        int result = 0,
            i;
        unsigned long aSize = a.size();
        if (aSize != b.size()) {
            cout << "Error: Vectors must have same size.";
            return -1;
        }
#pragma omp parallel for private(i) reduction(+:result)
        for (i = 0; i < aSize; ++i) {
            result += a[i] * b[i];
        }
        return result;
    }

    vector<vector<int> > HW1::MatrixSum(vector<vector<int> > &A, vector<vector<int> > &B) {
        unsigned long aRows = A.size(),
                      aCols = A[0].size();
        int i;
        vector<vector<int> > C(aRows, vector<int>(aCols, 0));
        if (aRows != B.size() || aCols != B[0].size()) {
            cout << "Error: Matrix must have same size.";
            return C;
        }
//#pragma omp parallel for private(i)
        for (i = 0; i < aRows; ++i) {
            unsigned long t = A[i].size();
            for (int j = 0; j < aCols; ++j) {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
        return C;
    }

    vector<vector<int> > HW1::MatrixProduct(vector<vector<int> > &A, vector<vector<int> > &B) {
        unsigned long aRows = A.size(),
                      aCols = A[0].size(),
                      bCols = B[0].size();
        vector<vector<int> > C(aRows, vector<int>(bCols, 0));
        int aRow;
        if (aCols != B.size()) {
            cout << "Error: Matrix A should have number of rows equal to the number of cols in the matrix B.";
            return C;
        }
//#pragma omp parallel for private(aRow)
        for (aRow = 0; aRow < aRows; ++aRow) {
            for (int bCol = 0; bCol < bCols; ++bCol) {
                for (int bRow = 0; bRow < bCols; ++bRow) {
                    C[aRow][bCol] += A[aRow][bRow] * B[bRow][bCol];
                }
            }
        }
        return C;
    }

}