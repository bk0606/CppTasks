#include <iostream>
#include <vector>
#include "parallel_tasks/hw3/HW3.cpp"

using namespace std;
using namespace parallel_tasks;

int main(int argc, char **argv) {
    int mtxRows = 3;
    int mtxRowsCols[3] = { 3, 5, 4 };
    int** mtx = new int* [mtxRows];

    for (int i = 0; i < mtxRows; ++i) {
        mtx[i] = new int[mtxRowsCols[i]];
        for (int j = 0; j < mtxRowsCols[i]; ++j) {
            mtx[i][j] = j;
        }
    }

    HW3::matrixElementsSum(mtx, mtxRows, mtxRowsCols);

    int tmp[] = {16, 2, 77, 29};
    vector<int> vect(tmp, tmp + sizeof(tmp) / sizeof(int));

    HW3::minMaxElements_critical(vect);

    HW3::minMaxElements_lock(vect);

    HW3::matrixTransformations_sections(mtx, mtxRows, mtxRowsCols);

    for (int k = 0; k < 2; k++)
        delete [] mtx[k];

    return 0;
}
