#ifndef INTEGRATE_H
#define INTEGRATE_H

#include <stddef.h>

namespace mathf {

    class Mathf {
    public:
        typedef float (*ptr_integrand)(float );

        static float integrateLRects(ptr_integrand func, float from, float to, int stepsCnt);

        static float integrateMRects(ptr_integrand func, float from, float to, int stepsCnt);

        static float integrateRRects(ptr_integrand func, float from, float to, int stepsCnt);
    };

}

#endif
