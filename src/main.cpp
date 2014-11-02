#include "parallel_tasks/HW5.cpp"
#include "mathf/Matrix.h"
#include "utils/Console.h"

using namespace parallel_tasks;
using namespace mathf;
using namespace utils;

int main (int argc, char** argv) {
    HW5::run(argc, argv);
//    int aArr[6] = {
//            1, 2, 3,
//            4, 5, 6
//    };
//    int bArr[4] = {
//            5, 6,
//            7, 8
//    };
//    Matrix a(aArr, 2, 3);
//    Matrix b = Matrix::transpose(a);
//    Console::printMatrix(b.begin(), b.rowsCnt, b.colsCnt);
    return 0;
}
