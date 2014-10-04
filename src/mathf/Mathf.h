#ifndef INTEGRATE_H
#define INTEGRATE_H

#include <stddef.h>

namespace mathf {

    class Mathf {
    public:
        typedef float (*ptr_integrandFunc)(float );

        static float integrateLRects(ptr_integrandFunc func, float from, float to, int stepsCnt);

        static float integrateMRects(ptr_integrandFunc func, float from, float to, int stepsCnt);

        static float integrateRRects(ptr_integrandFunc func, float from, float to, int stepsCnt);
    };

}

#endif
