#ifndef HW1_CPP
#define HW1_CPP

#include <vector>
#include <iostream>
#include <ctime>
#include <omp.h>
#include "../utils/Generator.h"
#include "../utils/Console.h"

using namespace std;
using namespace utils;

namespace parallel_tasks {

    class HW1 {
    public:
        static vector<int> SimpleVectorsProduct(vector<int> &a, vector<int> &b) {
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

        static int VectorsProductsSum(vector<int> &a, vector<int> &b) {
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

        static vector<vector<int> > MatrixSum(vector<vector<int> > &A, vector<vector<int> > &B) {
            unsigned long aRows = A.size(),
                    aCols = A[0].size();
            int i;
            vector<vector<int> > C(aRows, vector<int>(aCols, 0));
            if (aRows != B.size() || aCols != B[0].size()) {
                cout << "Error: Matrix must have same size.";
                return C;
            }
#pragma omp parallel for private(i)
            for (i = 0; i < aRows; ++i) {
                unsigned long t = A[i].size();
                for (int j = 0; j < aCols; ++j) {
                    C[i][j] = A[i][j] + B[i][j];
                }
            }
            return C;
        }

        static vector<vector<int> > MatrixProduct(vector<vector<int> > &A, vector<vector<int> > &B) {
            unsigned long aRows = A.size(),
                          bRows = B.size(),
                          aCols = A[0].size(),
                          bCols = B[0].size();
            vector<vector<int> > C(aRows, vector<int>(bCols, 0));
            if (aCols != bRows) {
                cout << "Error: Matrix A should have number of rows equal to the number of cols in the matrix B.";
                return C;
            }
            int aRow;
#pragma omp parallel for private(aRow)
            for (aRow = 0; aRow < aRows; ++aRow) {
                for (int bCol = 0; bCol < bCols; ++bCol) {
                    for (int bRow = 0; bRow < bRows; ++bRow) {
                        C[aRow][bCol] += A[aRow][bRow] * B[bRow][bCol];
                    }
                }
            }
            return C;
        }


        static void run(int vectorsSize, int matrixRows, int matrixCols) {
            int randDispersion = 10;

            vector<int> vectorA_ = Generator::generateVector(vectorsSize, randDispersion);
            vector<int> vectorB_ = Generator::generateVector(vectorsSize, randDispersion);
            cout << "Test vectors: \na: ";
            Console::printVector(vectorA_);
            cout << "b: ";
            Console::printVector(vectorB_);
            cout << "\n\n";

            vector<vector<int> > matrixA_ = Generator::generateMatrixVector(matrixRows, matrixCols, randDispersion);
            vector<vector<int> > matrixB_ = Generator::generateMatrixVector(matrixRows, matrixCols, randDispersion);
            cout << "Test matrix: \nA:\n";
            Console::printMatrix(matrixA_);
            cout << "\nB:\n";
            Console::printMatrix(matrixB_);
            cout << "\n\n";

            clock_t start = clock();
            cout << "Task #2: \nSimple vectors product: a x b = ";
            Console::printVector(SimpleVectorsProduct(vectorA_, vectorB_));
            cout << "\n\n";

            cout << "Task #3: \nMatrix sum: A + B = \n";
            Console::printMatrix(MatrixSum(matrixA_, matrixB_));
            cout << "\n\n";

            cout << "Task #4: \nMatrix product: A x B = \n";
            Console::printMatrix(MatrixProduct(matrixA_, matrixB_));
            cout << "\n\n";

            cout << "Task #5: \nVectors products sum: Sum(a x b) = "
                    << VectorsProductsSum(vectorA_, vectorB_) << "\n\n";
            cout << "\n\nExec time: " << (clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << endl;
        }
    };

}

#endif