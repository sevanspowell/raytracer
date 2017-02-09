#include <trc/SurfaceList.h>

namespace trc {
bool SurfaceList::hit(const Ray &ray, float tMin, float tMax,
                      HitRecord *recordOut) const {
    HitRecord tempRecord;
    bool hitAnything    = false;
    double closestSoFar = tMax;

    for (int i = 0; i < surfaces_.size(); ++i) {
        if (surfaces_[i]->hit(ray, tMin, closestSoFar, &tempRecord)) {
            hitAnything  = true;
            closestSoFar = tempRecord.t;
            *recordOut   = tempRecord;
        }
    }

    return hitAnything;
}

void SurfaceList::addSurface(const std::shared_ptr<Surface> &surface) {
    surfaces_.push_back(surface);
}

void SurfaceList::removeSurface(const std::shared_ptr<Surface> &surface) {
    // Try to find element to remove, swap with last element and remove last
    // element from vector
    for (size_t i = 0; i < surfaces_.size(); ++i) {
        if (surfaces_[i] == surface) {
            surfaces_[i] = surfaces_[surfaces_.size() - 1];
            surfaces_.pop_back();
        }
    }
}
}
