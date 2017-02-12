#pragma once

#include <trc/Ray.h>
#include <trc/Vec3.h>

namespace trc {
class Camera {
  public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, float vFovRad, float aspect) {
        Vec3 u, v, w;
        float theta = vFovRad;
        float halfHeight = tan(theta/2.0f);
        float halfWidth = aspect * halfHeight;
        origin_ = lookFrom;
        w = vec3::normalize(lookFrom - lookAt);
        u = vec3::normalize(vec3::cross(up, w));
        v = vec3::cross(w, u);
        lowerLeftCorner_ = origin_ - halfWidth*u - halfHeight*v - w;
        horizontal_ = 2.0f*halfWidth*u;
        vertical_ = 2.0f*halfHeight*v;
    }

    /// Return a ray to the given screen coords
    Ray getRay(float u, float v) {
        return Ray(origin_, lowerLeftCorner_ + u * horizontal_ + v * vertical_ -
                                origin_);
    };

  private:
    Vec3 origin_;
    Vec3 lowerLeftCorner_;
    Vec3 horizontal_;
    Vec3 vertical_;
};
}
