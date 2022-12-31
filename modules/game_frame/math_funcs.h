
#include "math_defs.h"
#include "core/math/vector3.h"
#include "core/typedefs.h"

class GFMath {

public:

    GFMath() {}

    static _ALWAYS_INLINE_ bool test(float p_test) { return p_test == 0.0f; }

    static Vector3 random_unit_vector();
};
