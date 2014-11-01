#include "parallel_tasks/HW5.cpp"
#include "mathf/Matrix.h"

using namespace parallel_tasks;
using namespace mathf;

#include <iostream>

int main (int argc, char** argv) {

//    Matrix m(1,1);
//    m(0,0) = 123;
//    int* arr = &m(0,0);
//    cout << arr[0];

//    int a[4] = {
//            1, 2,
//            3, 4
//    };
//    int b[4] = {
//            5, 6,
//            7, 8
//    };
//    Matrix aMtx(&a[0], 2, 2);
//    Matrix bMtx(&b[0], 2, 2);
//    cout << aMtx[0][0];

    HW5::run(argc, argv);
}
