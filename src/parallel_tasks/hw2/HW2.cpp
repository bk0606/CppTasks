#include <iostream>
#include <omp.h>
#include "HW2.h"
#include "../../utils/Console.h"
#include "../../utils/Generator.h"

using namespace mathf;
using namespace utils;

namespace parallel_tasks {

    void HW2::integrate(Mathf::ptr_integrand func, float from, float to, int stepsCnt) {
        cout << "\na) Left Rectangles = " << Mathf::integrateLRects(func, from, to, stepsCnt);
        cout << "\nb) Right Rectangles = " << Mathf::integrateRRects(func, from, to, stepsCnt);
        cout << "\nc) Middle Rectangles = " << Mathf::integrateMRects(func, from, to, stepsCnt);
    }

    void HW2::conditionParallel(int N) {
        vector<int> a = Generator::generateVector(N, 10);
        vector<int> b = Generator::generateVector(N, 10);
        cout << "a) ";
        bool isParallel = N > LIMIT;
        conditionParallel_(N, a, b, isParallel);

        cout << "b) ";
        isParallel = N % 2 == 0;
        conditionParallel_(N, a, b, isParallel);
    }

    void HW2::conditionParallel_(int n, vector<int> a, vector<int> b, bool isParallel) {
        vector<int> c(n, 0);
        int i;
#pragma omp parallel if(isParallel)
        {
#pragma omp single nowait
            {
                cout << "Parallel block uses: " << omp_get_num_threads() << " thread(s).\n";
            }
#pragma omp parallel for private(i)
            for (i = 0; i < n; ++i) {
                c[i] = (a[i] * b[i]);
            }
        }

    }

    void HW2::scheduleTest(int iterations) {
        vector<int> threads (iterations, 0);
        cout << "\nschedule(static): \n";
#pragma omp parallel for schedule(static)
        for (int i = 0; i < iterations; ++i) {
            threads[i] = omp_get_thread_num();
        }
        Console::printVector(threads);
        cout << "\nschedule(static, 4): \n";
#pragma omp parallel for schedule(static, 4)
        for (int i = 0; i < iterations; ++i) {
            threads[i] = omp_get_thread_num();
        }
        Console::printVector(threads);
        cout << "\nschedule(dynamic): \n";
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < iterations; ++i) {
            threads[i] = omp_get_thread_num();
        }
        Console::printVector(threads);
        cout << "\nschedule(dynamic, 3): \n";
#pragma omp parallel for schedule(dynamic, 3)
        for (int i = 0; i < iterations; ++i) {
            threads[i] = omp_get_thread_num();
        }
        Console::printVector(threads);
    }

}
