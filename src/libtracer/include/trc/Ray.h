#pragma once

#include <trc/Vec3.h>

namespace trc
{
class Ray
{
public:
    Ray()
    {
    }
    Ray(const Vec3 &origin, const Vec3 &dir) : origin_(origin), dir_(dir)
    {
    }

    const Vec3 &origin() const
    {
        return origin_;
    }
    const Vec3 &direction() const
    {
        return dir_;
    }

    /// \returns Point in space along the ray at the given parameter (e.g.
    /// time).
    Vec3 point_at_parameter(float t) const
    {
        return origin_ + t * dir_;
    }

private:
    Vec3 origin_;
    Vec3 dir_;
};
}
