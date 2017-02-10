#pragma once

#include <trc/Material.h>
#include <trc/Random.h>

namespace trc {
class LambertianMaterial : public Material {
  public:
    LambertianMaterial(const Vec3 &albedo) : albedo_(albedo) {}

    virtual bool scatter(const Ray &incidentRay, const HitRecord &hitRecord,
                         Vec3 *attenuationOut, Ray *scatteredRayOut) const {
        Vec3 target = hitRecord.hitPoint + hitRecord.normal +
                      trc_rnd::randomInUnitSphere();
        if (scatteredRayOut != nullptr) {
            *scatteredRayOut =
                Ray(hitRecord.hitPoint, target - hitRecord.hitPoint);
        }
        if (attenuationOut != nullptr) {
            *attenuationOut = albedo_;
        }
        return true;
    }

  private:
    Vec3 albedo_;
};
}
