#ifndef HW2_CPP
#define HW2_CPP

#include <iostream>
#include <omp.h>
#include "../utils/Console.h"
#include "../utils/Generator.h"
#include "../mathf/Mathf.h"

#define LIMIT 100

using namespace mathf;
using namespace utils;

namespace parallel_tasks {

    class HW2 {
    public:
        static void integrate(Mathf::ptr_integrandFunc func, float from, float to, int stepsCnt) {
            cout << "\na) Left Rectangles = " << Mathf::integrateLRects(func, from, to, stepsCnt);
            cout << "\nb) Right Rectangles = " << Mathf::integrateRRects(func, from, to, stepsCnt);
            cout << "\nc) Middle Rectangles = " << Mathf::integrateMRects(func, from, to, stepsCnt);
        }

        static void conditionParallel_(int n, vector<int> a, vector<int> b, bool isParallel) {
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

        static void conditionParallel(int N) {
            vector<int> a = Generator::generateVector(N, 10);
            vector<int> b = Generator::generateVector(N, 10);
            cout << "a) ";
            bool isParallel = N > LIMIT;
            conditionParallel_(N, a, b, isParallel);

            cout << "b) ";
            isParallel = N % 2 == 0;
            conditionParallel_(N, a, b, isParallel);
        }

        static void scheduleTest(int iterations) {
            vector<int> threads(iterations, 0);
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


        static float integrand(float x) {
            return x * x;
        };

        static void run() {
            clock_t start = clock();
            float from, to;
            int stepsCnt;
            cout << "\nTask #1. \nLower integrate bound = ";
            cin >> from;
            cout << "Upper integrate bound = ";
            cin >> to;
            cout << "Integration steps count = ";
            cin >> stepsCnt;
            cout << "\nintegrate(x*x, x, " << from << ", " << to << ")";
            integrate(&integrand, from, to, stepsCnt);

            int N;
            cout << "\n\nTask #2. N = ";
            cin >> N;
            conditionParallel(N);

            cout << "\n\nTask #3.";
            scheduleTest(25);
            cout << "\n\nExec time: " << (clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << endl;
        };
    };

}

#endif