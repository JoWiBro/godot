

#include "math_funcs.h"


Vector3 GFMath::random_unit_vector() {

    real_t phi = Math::random(0.0, RAD_360);
    real_t cos_theta = Math::random(-1.0f, 1.0f);

    real_t theta = Math::acos(cos_theta);

    return Vector3(
        Math::sin(theta) * Math::cos(phi),
        Math::sin(theta) * Math::sin(phi),
        Math::cos(theta)
    );
}

