#include <iostream>
#include <omp.h>
#include "tests/HW1Tests.cpp"
#include "tests/HW2Tests.cpp"
#include "utils/Console.h"

using namespace std;
using namespace tests;

int main() {
    HW1Tests hw1(300000000, 10, 10);
    hw1.run();

//    HW2Tests hw2;
//    hw2.run();

    return 0;
}