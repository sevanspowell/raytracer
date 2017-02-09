#pragma once

#include <trc/Ray.h>
#include <trc/Vec3.h>

namespace trc {
class Camera {
  public:
    Camera(const Vec3 &origin, const trc::Vec3 &lowerLeftCorner,
           const Vec3 &horizontal, const trc::Vec3 &vertical)
        : origin_(origin), lowerLeftCorner_(lowerLeftCorner),
          horizontal_(horizontal), vertical_(vertical) {}

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
