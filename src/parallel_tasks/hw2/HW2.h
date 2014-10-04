#ifndef HW2_H
#define HW2_H

#include <vector>
#include "../../mathf/Mathf.h"

using namespace mathf;
using namespace std;


namespace parallel_tasks {

    #define LIMIT 100

    class HW2 {
    public:
        void integrate(Mathf::ptr_integrandFunc func, float from, float to, int stepsCnt);

        void conditionParallel(int N);

        void scheduleTest(int iterations);

    private:
        void conditionParallel_(int n, vector<int> a, vector<int> b, bool isParallel);
    };

}

#endif