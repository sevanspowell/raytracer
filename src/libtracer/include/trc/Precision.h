#pragma once

#include <limits>

namespace trc
{
typedef float scalar;

const float SCALAR_MAX = std::numeric_limits<scalar>::max();

/**
 * How 'close' two floats need to be in order to be considered equal.
 */
const float FLOAT_ACCURACY = 10.0e-6f;
}
