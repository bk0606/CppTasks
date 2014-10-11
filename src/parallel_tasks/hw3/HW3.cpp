#include <iostream>
#include <vector>
#include <limits.h>
#include <omp.h>

using namespace std;

namespace parallel_tasks {
    class HW3 {
    public:
        static void matrixElementsSum(int** mtx, int rows, int cols[]) {
            // TODO: check - is mtx pass by ref or copied?
            int sum = 0;
#pragma omp parallel for shared(sum)
            for (int row = 0; row < rows; ++row) {
                if (row % 2 == 0) {
                    for (int col = 0; col < cols[row]; ++col) {
                        if (mtx[row][col] % 2 == 0) {
                            #pragma omp atomic
                                sum += mtx[row][col];
                        }
                    }
                }
                else {
                    for (int col = 0; col < cols[row]; ++col) {
                        if (mtx[row][col] % 2 != 0) {
                            #pragma omp atomic
                                sum += mtx[row][col];
                        }
                    }
                }
            }
            cout << "\nSum of matrix elements: " << sum << "\n\n";
        }

        static void minMaxElements_critical(vector<int> &vect) {
            int maxEl = INT_MIN, minEl = INT_MAX;
            unsigned long size = vect.size();
#pragma omp parallel for shared(maxEl, minEl)
            for (int i = 0; i < size; ++i) {
                int currEl = vect[i];
                if (i % 2 == 0) {
                    if (currEl > maxEl) {
#pragma omp critical
                        {
                            maxEl = currEl > maxEl ? currEl : maxEl;
                        }
                    }
                } else {
                    if (currEl < minEl) {
#pragma omp critical
                        {
                            minEl = currEl < minEl ? currEl : minEl;
                        }
                    }
                }
            }
            cout << "Critical zone. \nMin element: " << minEl << ", Max element: " << maxEl << "\n\n";
        }

        static void minMaxElements_lock(vector<int> &vect) {
            int maxEl = INT_MIN, minEl = INT_MAX;
            unsigned long size = vect.size();
            omp_lock_t lock;
            omp_init_lock(&lock);
#pragma omp parallel for shared(maxEl, minEl)
            for (int i = 0; i < size; ++i) {
                int currEl = vect[i];
                if (i % 2 == 0) {
                    if (currEl > maxEl) {
                        omp_set_lock(&lock);
                        maxEl = currEl > maxEl ? currEl : maxEl;
                        omp_unset_lock(&lock);
                    }
                } else {
                    if (currEl < minEl) {
                        omp_set_lock(&lock);
                        minEl = currEl < minEl ? currEl : minEl;
                        omp_unset_lock(&lock);
                    }
                }
            }
            omp_destroy_lock(&lock);
            cout << "Lock.\nMin element: " << minEl << ", Max element: " << maxEl << "\n\n";
        }

        static void matrixTransformations_sections(int** mtx, int rows, int cols[]) {
            float arithmeticMean = 0;
            int maxEl = INT_MIN, minEl = INT_MAX,
                multiple3ElemsCnt = 0,
                elemsCnt = 0;
            cout << "Task With sections.";
#pragma omp parallel sections
            {
#pragma section
                {
                    for (int i = 0; i < rows; ++i) {
                        for (int j = 0; j < cols[i]; ++j) {
                            arithmeticMean += mtx[i][j];
                            elemsCnt += 1;
                        }
                    }
                    cout << "\nArithmetical mean of matrix: " << arithmeticMean / elemsCnt;
                }
#pragma section
                {
                    for (int i = 0; i < rows; ++i) {
                        for (int j = 0; j < cols[i]; ++j) {
                            int currEl = mtx[i][j];
                            if (currEl > maxEl) {
                                maxEl = currEl;
                            }
                            if (currEl < minEl) {
                                minEl = currEl;
                            }
                        }
                    }
                    cout << "\nMin element of matrix: " << minEl << ", Max element of matrix: " << maxEl;
                }
#pragma section
                {
                    for (int i = 0; i < rows; ++i) {
                        for (int j = 0; j < cols[i]; ++j) {
                            if (mtx[i][j] % 3 == 0) {
                                multiple3ElemsCnt += 1;
                            }
                        }
                    }
                    cout << "\nCount of multiple 3 elements: " << multiple3ElemsCnt;
                }
            }
        }

    };
}