#include <algorithm>
#include <cmath>
#include <iostream>

#include <trc/Ray.h>
#include <trc/Vec3.h>

/// Convert a float in the range [0.0f - 1.0f] to a byte in the range [0 - 255].
/// Modified from
/// http://stackoverflow.com/questions/1914115/converting-color-value-from-float-0-1-to-byte-0-255
/// (Mark Byers)
inline int floatToByte(float f)
{
    float temp = std::max(0.0, std::min(1.0, (double)f));
    return (floor(f == 1.0 ? 255 : f * 256.0));
}

/// Return a gamma corrected copy of the \p colorValue.
inline float gammaCorrect(float colorValue, float gamma)
{
    return pow(colorValue, 1.0 / gamma);
}

float hit_sphere(const trc::Vec3 &center, float radius, const trc::Ray &ray)
{
    const trc::Vec3 oc = ray.origin() - center;
    float a = trc::vec3::dot(ray.direction(), ray.direction());
    float b = 2.0f * trc::vec3::dot(oc, ray.direction());
    float c = trc::vec3::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    return (discriminant > 0);
}

trc::Vec3 color_lerp(const trc::Ray &ray)
{
    trc::Vec3 unitDir = trc::vec3::normalize(ray.direction());
    // Convert -1.0 < y < 1.0 to 0.0 < t < 1.0
    float t = 0.5 * (unitDir.y + 1.0);
    // Linearly blend white and blue based on y-parameter t
    // Linear interpolation formula = (1 - t) * start_value + t * end_value
    return (1.0 - t) * trc::Vec3(1.0f, 1.0f, 1.0f) +
           t * trc::Vec3(0.5f, 0.7f, 1.0f);
}

trc::Vec3 color_sphere(const trc::Ray &ray)
{
    const trc::Vec3 sphereOrigin(0.0f, 0.0f, -1.0f);

    // Get how far to move along ray before hitting sphere
    float t = hit_sphere(sphereOrigin, 0.5f, ray);
    if (t > 0.0) {
        // Construct normal using hit point
        const trc::Vec3 normal =
            trc::vec3::normalize(ray.point_at_parameter(t) - sphereOrigin);
        // Put on range [0,1]
        return 0.5f * trc::Vec3(normal.x + 1.0f, normal.y + 1.0f,
                                normal.z + 1.0f);
    } else {
        return color_lerp(ray);
    }

}

int main()
{
    int nx = 200;
    int ny = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    const trc::Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    const trc::Vec3 horizontal(4.0f, 0.0f, 0.0f);
    const trc::Vec3 vertical(0.0f, 2.0f, 0.0f);
    const trc::Vec3 origin(0.0f, 0.0f, 0.0f);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            trc::Ray ray(origin,
                         lowerLeftCorner + u * horizontal + v * vertical);
            trc::Vec3 col = color_sphere(ray);
            int ir = floatToByte(col.x);
            int ig = floatToByte(col.y);
            int ib = floatToByte(col.z);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
