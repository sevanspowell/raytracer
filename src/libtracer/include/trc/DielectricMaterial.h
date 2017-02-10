#pragma once

#include <trc/Material.h>
#include <trc/MetalMaterial.h>

namespace trc {
bool refract(const Vec3 &v, const Vec3 &n, float niOverNt, Vec3 *refractedOut) {
    Vec3 uv            = vec3::normalize(v);
    float dt           = vec3::dot(uv, n);
    float discriminant = 1.0f - niOverNt * niOverNt * (1 - dt * dt);
    if (discriminant > 0) {
        *refractedOut = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

float schlick(float cosine, float refractiveIndex) {
    float r0 = (1.0f - refractiveIndex) / (1.0f + refractiveIndex);
    r0       = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

class DielectricMaterial : public Material {
  public:
    DielectricMaterial(float refractiveIndex)
        : refractiveIndex_(refractiveIndex) {}

    virtual bool scatter(const Ray &incidentRay, const HitRecord &hitRecord,
                         Vec3 *attenuationOut, Ray *scatteredRayOut) const {
        assert(attenuationOut != nullptr && scatteredRayOut != nullptr);

        Vec3 outwardNormal;
        Vec3 reflected = reflect(incidentRay.direction(), hitRecord.normal);
        float niOverNt;
        *attenuationOut = Vec3(1.0f, 1.0f, 1.0f);
        Vec3 refracted;
        float reflectProbability;
        float cosine;

        if (vec3::dot(incidentRay.direction(), hitRecord.normal) > 0) {
            outwardNormal = -hitRecord.normal;
            niOverNt      = refractiveIndex_;
            cosine = vec3::dot(incidentRay.direction(), hitRecord.normal) /
                     vec3::length(incidentRay.direction());
            cosine = sqrt(1.0f -
                          refractiveIndex_ * refractiveIndex_ *
                              (1.0f - cosine * cosine));
        } else {
            outwardNormal = hitRecord.normal;
            niOverNt      = 1.0f / refractiveIndex_;
            cosine = -vec3::dot(incidentRay.direction(), hitRecord.normal) /
                     vec3::length(incidentRay.direction());
        }

        if (refract(incidentRay.direction(), outwardNormal, niOverNt,
                    &refracted)) {
            reflectProbability = schlick(cosine, refractiveIndex_);
        } else {
            reflectProbability = 1.0f;
        }

        if (trc_rnd::dist(trc_rnd::mt) < reflectProbability) {
            *scatteredRayOut = Ray(hitRecord.hitPoint, reflected);
        } else {
            *scatteredRayOut = Ray(hitRecord.hitPoint, refracted);
        }

        return true;
    }

  private:
    float refractiveIndex_;
};
}
