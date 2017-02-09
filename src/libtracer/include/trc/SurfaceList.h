#pragma once

#include <memory>
#include <vector>

#include <trc/Surface.h>

namespace trc {
class SurfaceList : public Surface {
  public:
    virtual bool hit(const Ray &ray, float tMin, float tMax,
                     HitRecord *recordOut) const;

    void addSurface(const std::shared_ptr<Surface> &surface);
    void removeSurface(const std::shared_ptr<Surface> &surface);

  private:
    std::vector<std::shared_ptr<Surface>> surfaces_;
};
}
