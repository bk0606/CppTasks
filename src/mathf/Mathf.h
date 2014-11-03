#ifndef INTEGRATE_H
#define INTEGRATE_H

#include <stddef.h>

#define EPSILON 0.1

namespace mathf {

    class Mathf {
    public:
        static float min(float a, float b);

        typedef float (*ptr_integrandFunc)(float );
        static float integrateLRects(ptr_integrandFunc func, float from, float to, int stepsCnt);
        static float integrateMRects(ptr_integrandFunc func, float from, float to, int stepsCnt);
        static float integrateRRects(ptr_integrandFunc func, float from, float to, int stepsCnt);
    };

}

#endif
