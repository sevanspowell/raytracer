#pragma once

#include <cassert>

#include <trc/Material.h>
#include <trc/Random.h>

namespace trc {
Vec3 reflect(const Vec3 &v, const Vec3 &normal) {
    return (v - 2 * vec3::dot(v, normal) * normal);
}

class MetalMaterial : public Material {
  public:
    MetalMaterial(const Vec3 &albedo, float fuzziness)
        : albedo_(albedo), fuzziness_(fuzziness) {}

    virtual bool scatter(const Ray &incidentRay, const HitRecord &hitRecord,
                         Vec3 *attenuationOut, Ray *scatteredRayOut) const {
        assert(scatteredRayOut != nullptr && attenuationOut != nullptr);
        Vec3 reflected =
            reflect(vec3::normalize(incidentRay.direction()), hitRecord.normal);
        *scatteredRayOut =
            Ray(hitRecord.hitPoint,
                reflected + fuzziness_ * trc_rnd::randomInUnitSphere());
        *attenuationOut = albedo_;
        return (vec3::dot(scatteredRayOut->direction(), hitRecord.normal) > 0);
    }

  private:
    Vec3 albedo_;
    float fuzziness_; // Radius of sphere from which we choose a random point, 0
                      // means perfectly reflected ray
};
}
