#include "Mathf.h"

namespace mathf {

    float Mathf::min(float a, float b) {
        return a > b ? b : a;
    }

    float Mathf::integrateLRects(ptr_integrandFunc func, float from, float to, int stepsCnt) {
        float result = 0,
              step = (to - from)/ stepsCnt;
#pragma omp parallel for reduction(+:result)
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(from+step*i) * ((from+step*(i+1)) - (from+step*i));
        }
        return result;
    }

    float Mathf::integrateMRects(Mathf::ptr_integrandFunc func, float from, float to, int stepsCnt) {
        float result = 0,
                step = (to - from)/ stepsCnt;
#pragma omp parallel for reduction(+:result)
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(((from+step*(i-1)) + (from+step*i))/2) * ((from+step*i) - (from+step*(i-1)));
        };
        return result;
    }

    float Mathf::integrateRRects(Mathf::ptr_integrandFunc func, float from, float to, int stepsCnt) {
        float result = 0,
                step = (to - from)/ stepsCnt;
#pragma omp parallel for reduction(+:result)
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(from+step*i) * ((from+step*i) - (from+step*(i-1)));
        };
        return result;
    }
}
