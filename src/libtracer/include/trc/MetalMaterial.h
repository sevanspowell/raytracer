#pragma once

#include <trc/Material.h>

namespace trc {
Vec3 reflect(const Vec3 &v, const Vec3 &normal) {
    return (v - 2 * vec3::dot(v, normal) * normal);
}

class MetalMaterial : public Material {
  public:
    MetalMaterial(const Vec3 &albedo) : albedo_(albedo) {}

    virtual bool scatter(const Ray &incidentRay, const HitRecord &hitRecord,
                         Vec3 *attenuationOut, Ray *scatteredRayOut) const {
        Vec3 reflected = reflect(vec3::normalize(incidentRay.direction()),
                                    hitRecord.normal);
        if (scatteredRayOut != nullptr) {
            *scatteredRayOut = Ray(hitRecord.hitPoint, reflected);
        }
        if (attenuationOut != nullptr) {
            *attenuationOut = albedo_;
        }
        return (vec3::dot(Ray(hitRecord.hitPoint, reflected).direction(),
                          hitRecord.normal) > 0);
    }

  private:
    Vec3 albedo_;
};
}
