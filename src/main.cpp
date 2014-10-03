#include <iostream>
#include "tests/HW1Tests.cpp"
#include "tests/HW2Tests.cpp"

using namespace std;
using namespace tests;

int main() {
//    HW1Tests hw1(100000, 10, 10);
//    hw1.run();

    HW2Tests hw2;
    hw2.run();

    return 0;
}