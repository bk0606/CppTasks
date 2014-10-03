#include "Mathf.h"

namespace mathf {

    float Mathf::integrateLRects(ptr_integrand func, float from, float to, int stepsCnt) {
        float result = 0,
              step = (to - from)/ stepsCnt;
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(from +step*i) * ((from +step*(i+1)) - (from +step*i));
        }
        return result;
    }

    float Mathf::integrateMRects(Mathf::ptr_integrand func, float from, float to, int stepsCnt) {
        float result = 0,
                step = (to - from)/ stepsCnt;
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(((from +step*(i-1)) + (from +step*i))/2) * ((from +step*i) - (from +step*(i-1)));
        };
        return result;
    }

    float Mathf::integrateRRects(Mathf::ptr_integrand func, float from, float to, int stepsCnt) {
        float result = 0,
                step = (to - from)/ stepsCnt;
        for (int i = 1; i <= stepsCnt; ++i) {
            result += func(from +step*i) * ((from +step*i) - (from +step*(i-1)));
        };
        return result;
    }
}
