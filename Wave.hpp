//
// Created by trima on 21/04/18.
//

#ifndef RAY_TRACER_WATER_HPP
#define RAY_TRACER_WATER_HPP

/// Namespace RayTracer

#include "PointVector.h"

namespace rt {
    struct Wave {
        Real phase;
        Real r;
        Real a;
        Real l;

        Wave(){};
        Wave(Real r, Real a, Real l, Real phase ):phase(phase),r(r),a(a),l(l){

        }

    };
}

#endif //RAY_TRACER_WATER_HPP
