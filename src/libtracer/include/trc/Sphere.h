#pragma once

#include <trc/Surface.h>

namespace trc
{
class Sphere : public Surface
{
public:
    Sphere()
    {
    }
    Sphere(const trc::Vec3 &center, const float radius)
        : center_(center), radius_(radius)
    {
    }

    virtual bool
    hit(const Ray &ray, float tMin, float tMax, HitRecord *recordOut) const;

    const trc::Vec3 &center() const;
    const float &radius() const;

private:
    trc::Vec3 center_;
    float radius_;
};
}
