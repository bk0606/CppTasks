#include <iostream>
#include "../parallel_tasks/hw2/HW2.h"

using namespace std;
using namespace parallel_tasks;

namespace tests {

    class HW2Tests {
    public:

        void run() {
            clock_t start = clock();
            task1();
            task2();
            task3();
            cout << "\n\nExec time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
        };

        static float integrand(float x) {
            return x*x;
        }

        void task1() {
            float from, to;
            int stepsCnt;
            cout << "\nTask #1. \nLower integrate bound = ";
            cin >> from;
            cout << "Upper integrate bound = ";
            cin >> to;
            cout << "Integration steps count = ";
            cin >> stepsCnt;
            cout << "\nintegrate(x*x, x, " << from << ", " << to << ")";
            hw2_.integrate(&integrand, from, to, stepsCnt);
        };

        void task2() {
            int N;
            cout << "\n\nTask #2. N = ";
            cin >> N;
            hw2_.conditionParallel(N);
        };

        void task3() {
            cout << "\n\nTask #3.";
            hw2_.scheduleTest(25);
        };


        HW2Tests() {}

        ~HW2Tests() {}

    private:
        HW2 hw2_;
    };

}