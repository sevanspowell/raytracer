#pragma once

#include <trc/Precision.h>
#include <trc/Ray.h>
#include <trc/Vec3.h>

namespace trc
{
struct HitRecord {
    scalar t;
    Vec3 hitPoint;
    Vec3 normal;
};

/// Interface for any object that can be hit by a ray
class Surface
{
public:
    /// \param   [out]   recordOut   Hit information out.
    /// \returns True if the given \p ray hits the surface at some point in the
    /// parameter interval (\tMin, \tMax).
    virtual bool
    hit(const Ray &ray, float tMin, float tMax, HitRecord *recordOut) const = 0;
};
}
