#include <stdlib.h>
#include <ctime>
#include <iostream>
#include "../parallel_tasks/hw1/HW1.h"
#include "../utils/Console.h"
#include "../utils/Generator.h"

using namespace parallel_tasks;
using namespace utils;

namespace tests {

    class HW1Tests {
    public:

        void run() {
            clock_t start = clock();
            task2();
            task3();
            task4();
            task5();
            cout << "\n\nExec time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
        };

        void task2() {
            cout << "Task #2: \nSimple vectors product: a x b = ";
            Console::printVector(hw1.SimpleVectorsProduct(vectorA_, vectorB_));
            cout << "\n\n";
        }

        void task3() {
            cout << "Task #3: \nMatrix sum: A + B = \n";
            Console::printMatrix(hw1.MatrixSum(matrixA_, matrixB_));
            cout << "\n\n";
        }

        void task4() {
            cout << "Task #4: \nMatrix product: A x B = \n";
            Console::printMatrix(hw1.MatrixProduct(matrixA_, matrixB_));
            cout << "\n\n";
        }

        void task5() {
            cout << "Task #5: \nVectors products sum: Sum(a x b) = "
                    << hw1.VectorsProductsSum(vectorA_, vectorB_) << "\n\n";
        }

        HW1Tests(int const vectorsSize, int const matrixRows, int const matrixCols)
                : vectorsSize_(vectorsSize), matrixRows_(matrixRows), matrixCols_(matrixCols) {
            int dispersion = 10;
            vectorA_ = Generator::generateVector(vectorsSize, dispersion);
            vectorB_ = Generator::generateVector(vectorsSize, dispersion);
            cout << "Test vectors: \na: ";
            Console::printVector(vectorA_);
            cout << "b: ";
            Console::printVector(vectorB_);
            cout << "\n\n";

            matrixA_ = Generator::generateMatrix(matrixRows, matrixCols, dispersion);
            matrixB_ = Generator::generateMatrix(matrixRows, matrixCols, dispersion);
            cout << "Test matrix: \nA:\n";
            Console::printMatrix(matrixA_);
            cout << "\nB:\n";
            Console::printMatrix(matrixB_);
            cout << "\n\n";

            hw1 = HW1();
        }

        ~HW1Tests() {
            vectorA_.clear();
            vectorB_.clear();
            matrixA_.clear();
            matrixB_.clear();
        }

    private:
        const int vectorsSize_;
        const int matrixRows_;
        const int matrixCols_;
        vector<int> vectorA_;
        vector<int> vectorB_;
        vector<vector<int> > matrixA_;
        vector<vector<int> > matrixB_;
        HW1 hw1;
    };

}
