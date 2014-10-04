#include "Mathf.h"

namespace mathf {

    float Mathf::integrateLRects(ptr_integrandFunc func, float from, float to, int stepsCnt) {
        float result = 0,
              step = (to - from)/ stepsCnt;
        // i специально не выносил из блока, чтобы сделать private(i),
        // т.к. объявленная внутри блока переменная является private по умолчанию
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
