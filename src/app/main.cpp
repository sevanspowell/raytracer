#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>

#include <trc/Camera.h>
#include <trc/Ray.h>
#include <trc/Sphere.h>
#include <trc/SurfaceList.h>
#include <trc/Vec3.h>

using namespace trc;

/// Convert a float in the range [0.0f - 1.0f] to a byte in the range [0 - 255].
/// Modified from
/// http://stackoverflow.com/questions/1914115/converting-color-value-from-float-0-1-to-byte-0-255
/// (Mark Byers)
inline int floatToByte(float f) {
    float temp = std::max(0.0, std::min(1.0, (double)f));
    return (floor(f == 1.0 ? 255 : f * 256.0));
}

/// Return a gamma corrected copy of the \p colorValue.
inline float gammaCorrect(float colorValue, float gamma) {
    return pow(colorValue, 1.0 / gamma);
}

inline Vec3 gammaCorrect(Vec3 color, float gamma) {
    return (Vec3(gammaCorrect(color.x, gamma), gammaCorrect(color.y, gamma),
                 gammaCorrect(color.z, gamma)));
}

float hit_sphere(const Vec3 &center, float radius, const Ray &ray) {
    const Vec3 oc      = ray.origin() - center;
    float a            = vec3::dot(ray.direction(), ray.direction());
    float b            = 2.0f * vec3::dot(oc, ray.direction());
    float c            = vec3::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    return (discriminant > 0);
}

Vec3 color_lerp(const Ray &ray) {
    Vec3 unitDir = vec3::normalize(ray.direction());
    // Convert -1.0 < y < 1.0 to 0.0 < t < 1.0
    float t = 0.5 * (unitDir.y + 1.0);
    // Linearly blend white and blue based on y-parameter t
    // Linear interpolation formula = (1 - t) * start_value + t * end_value
    return (1.0 - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

Vec3 color_normal(const Ray &ray) {
    const Vec3 sphereOrigin(0.0f, 0.0f, -1.0f);

    // Get how far to move along ray before hitting sphere
    float t = hit_sphere(sphereOrigin, 0.5f, ray);
    if (t > 0.0) {
        // Construct normal using hit point
        const Vec3 normal =
            vec3::normalize(ray.pointAtParameter(t) - sphereOrigin);
        // Put on range [0,1]
        return 0.5f * Vec3(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f);
    } else {
        return color_lerp(ray);
    }
}

Vec3 color(const Ray &ray, const std::shared_ptr<Surface> &world) {
    HitRecord rec;
    if (world->hit(ray, 0.0f, SCALAR_MAX, &rec)) {
        return 0.5f * Vec3(rec.normal.x + 1.0f, rec.normal.y + 1.0f,
                           rec.normal.z + 1.0f);
    } else {
        return color_lerp(ray);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100; // Number of samples to take for each pixel

    std::random_device randDevice;
    std::mt19937 mt(randDevice());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    const Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    const Vec3 horizontal(4.0f, 0.0f, 0.0f);
    const Vec3 vertical(0.0f, 2.0f, 0.0f);
    const Vec3 origin(0.0f, 0.0f, 0.0f);
    Camera cam(origin, lowerLeftCorner, horizontal, vertical);

    std::shared_ptr<SurfaceList> world(new SurfaceList());
    world->addSurface(
        std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f)));
    world->addSurface(std::shared_ptr<Sphere>(
        new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f)));

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);
            // Sum samples
            for (int s = 0; s < ns; ++s) {
                float u = float(i + dist(mt)) / float(nx);
                float v = float(j + dist(mt)) / float(ny);
                Ray ray = cam.getRay(u, v);
                Vec3 p  = ray.pointAtParameter(2.0f);
                col += color(ray, world);
            }

            // Average samples
            col /= float(ns);

            int ir = floatToByte(col.x);
            int ig = floatToByte(col.y);
            int ib = floatToByte(col.z);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
