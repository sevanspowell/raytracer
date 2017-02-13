#pragma once

#include <trc/Random.h>
#include <trc/Ray.h>
#include <trc/Vec3.h>

namespace trc {
/// For defocus blur
Vec3 randomInUnitDisk() {
    Vec3 p;

    do {
        p = 2.0f *
                Vec3(trc_rnd::getRandom0To1(), trc_rnd::getRandom0To1(), 0.0f) -
            Vec3(1.0f, 1.0f, 0.0f);
    } while (vec3::dot(p, p) >= 1.0f);

    return p;
}

class Camera {
  public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, float vFovRad, float aspect,
           float aperture, float focusDist) {
        lensRadius_      = aperture / 2.0f;
        float theta      = vFovRad;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth  = aspect * halfHeight;
        origin_          = lookFrom;
        w_               = vec3::normalize(lookFrom - lookAt);
        u_               = vec3::normalize(vec3::cross(up, w_));
        v_               = vec3::cross(w_, u_);
        lowerLeftCorner_ = origin_ - halfWidth * focusDist * u_ -
                           halfHeight * focusDist * v_ - focusDist * w_;
        horizontal_ = 2.0f * halfWidth * focusDist * u_;
        vertical_   = 2.0f * halfHeight * focusDist * v_;
    }

    /// Return a ray to the given screen coords
    Ray getRay(float s, float t) {
        Vec3 rd     = lensRadius_ * randomInUnitDisk();
        Vec3 offset = u_ * rd.x + v_ * rd.y;
        return Ray(origin_ + offset, lowerLeftCorner_ + s * horizontal_ +
                                         t * vertical_ - origin_ - offset);
    };

  private:
    Vec3 origin_;
    Vec3 lowerLeftCorner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_, v_, w_;
    float lensRadius_;
};
}
