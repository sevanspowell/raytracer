#pragma once

#include <trc/Ray.h>
#include <trc/Surface.h>

namespace trc {
/// A material is a reflector of light
class Material {
  public:
    /// \param     incidentRay
    /// \param     hitRecord
    /// \param     [out] attenuationOut    How much ray should be attenuated.
    /// \param     [out] scatteredRayOut
    /// \returns   True if incident ray is scattered, false otherwise. 
    virtual bool scatter(const Ray &incidentRay, const HitRecord &hitRecord,
                         Vec3 *attenuationOut, Ray *scatteredRayOut) const = 0;
};
}
