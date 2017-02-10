#include <cmath>

#include <trc/Sphere.h>

namespace trc {
bool Sphere::hit(const Ray &ray, float tMin, float tMax,
                 HitRecord *recordOut) const {
    bool result = false;

    const Vec3 oc = ray.origin() - center_;
    const float a = vec3::dot(ray.direction(), ray.direction());
    const float b = 2.0f * vec3::dot(oc, ray.direction());
    const float c = vec3::dot(oc, oc) - radius_ * radius_;

    const float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / (2.0f * a);
        if (temp < tMax && temp > tMin) {
            if (recordOut != nullptr) {
                recordOut->t        = temp;
                recordOut->hitPoint = ray.pointAtParameter(recordOut->t);
                recordOut->normal =
                    vec3::normalize((recordOut->hitPoint - center_));
                recordOut->material = material_;
            }

            result = true;
        } else {
            temp = (-b + sqrt(discriminant)) / (2.0f * a);
            if (temp < tMax && temp > tMin) {
                if (recordOut != nullptr) {
                    recordOut->t        = temp;
                    recordOut->hitPoint = ray.pointAtParameter(recordOut->t);
                    recordOut->normal =
                        vec3::normalize((recordOut->hitPoint - center_));
                    recordOut->material = material_;
                }

                result = true;
            }
        }
    }

    return result;
}

const trc::Vec3 &Sphere::center() const { return center_; }

const float &Sphere::radius() const { return radius_; }
}
