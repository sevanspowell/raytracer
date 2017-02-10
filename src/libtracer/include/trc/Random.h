#pragma once

#include <random>

#include <trc/Vec3.h>

namespace trc_rnd {
std::random_device randDevice;
std::mt19937 mt(randDevice());
std::uniform_real_distribution<double> dist(0.0, 1.0);

/// Rejection method for getting a random point inside a unit sphere.
trc::Vec3 randomInUnitSphere() {
    trc::Vec3 pt;

    // Pick a random point in a cube and reject it if outside of sphere
    do {
        pt = 2.0f * trc::Vec3(dist(mt), dist(mt), dist(mt)) -
             trc::Vec3(1.0f, 1.0f, 1.0f);
    } while (trc::vec3::squared_length(pt) >= 1.0f);

    return pt;
}
}
