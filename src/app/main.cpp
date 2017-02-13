#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>

#include <trc/Camera.h>
#include <trc/DielectricMaterial.h>
#include <trc/LambertianMaterial.h>
#include <trc/MetalMaterial.h>
#include <trc/Ray.h>
#include <trc/Sphere.h>
#include <trc/SurfaceList.h>
#include <trc/Vec3.h>

using namespace trc;

std::random_device randDevice;
std::mt19937 mt(randDevice());
std::uniform_real_distribution<double> dist(0.0, 1.0);

inline float degToRad(float degrees) { return ((degrees * M_PI) / 180.0f); }

/// Rejection method for getting a random point inside a unit sphere.
Vec3 randomInUnitSphere() {
    Vec3 pt;

    // Pick a random point in a cube and reject it if outside of sphere
    do {
        pt = 2.0f * Vec3(dist(mt), dist(mt), dist(mt)) - Vec3(1.0f, 1.0f, 1.0f);
    } while (vec3::squared_length(pt) >= 1.0f);

    return pt;
}

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

/// \param   depth   Used to indicate number of recursions
Vec3 color(const Ray &ray, const std::shared_ptr<Surface> &world, int depth) {
    HitRecord rec;
    // Ignore hits very close to 0
    if (world->hit(ray, 0.001f, SCALAR_MAX, &rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 &&
            rec.material->scatter(ray, rec, &attenuation, &scattered)) {
            return vec3::multiply(attenuation,
                                  color(scattered, world, depth + 1));
        } else {
            return Vec3(0.0f, 0.0f, 0.0f);
        }
    } else {
        return color_lerp(ray);
    }
}

std::shared_ptr<SurfaceList> getRandomScene() {
    int n = 500;
    std::shared_ptr<SurfaceList> world(new SurfaceList());
    world->addSurface(std::shared_ptr<Sphere>(
        new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                   std::shared_ptr<LambertianMaterial>(
                       new LambertianMaterial(Vec3(0.5f, 0.5f, 0.5f))))));
    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float chooseMat = trc_rnd::getRandom0To1();
            Vec3 center(a + 0.9f * trc_rnd::getRandom0To1(), 0.2f,
                        b + 0.9f * trc_rnd::getRandom0To1());
            if (vec3::length(center - Vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                if (chooseMat < 0.8f) { // Diffuse
                    world->addSurface(std::shared_ptr<Sphere>(new Sphere(
                        center, 0.2f,
                        std::shared_ptr<LambertianMaterial>(
                            new LambertianMaterial(Vec3(
                                trc_rnd::getRandom0To1() * trc_rnd::getRandom0To1(),
                                trc_rnd::getRandom0To1() * trc_rnd::getRandom0To1(),
                                trc_rnd::getRandom0To1() *
                                    trc_rnd::getRandom0To1()))))));
                } else if (chooseMat < 0.95f) { // Metal
                    world->addSurface(std::shared_ptr<Sphere>(new Sphere(
                        center, 0.2f,
                        std::shared_ptr<MetalMaterial>(new MetalMaterial(
                            Vec3(0.5f * (1.0f + trc_rnd::getRandom0To1()),
                                 0.5f * (1.0f + trc_rnd::getRandom0To1()),
                                 0.5f * (1.0f + trc_rnd::getRandom0To1())),
                            0.5f * trc_rnd::getRandom0To1())))));
                } else { // Glass
                    world->addSurface(std::shared_ptr<Sphere>(new Sphere(
                        center, 0.2f, std::shared_ptr<DielectricMaterial>(
                                          new DielectricMaterial(1.5f)))));
                }
            }
        }
    }

    world->addSurface(std::shared_ptr<Sphere>(new Sphere(
        Vec3(0.0f, 1.0f, 0.0f), 1.0f,
        std::shared_ptr<DielectricMaterial>(new DielectricMaterial(1.5f)))));
    world->addSurface(std::shared_ptr<Sphere>(
        new Sphere(Vec3(-4.0f, 1.0f, 0), 1.0f,
                   std::shared_ptr<LambertianMaterial>(
                       new LambertianMaterial(Vec3(0.4f, 0.2f, 0.1f))))));
    world->addSurface(std::shared_ptr<Sphere>(
        new Sphere(Vec3(4.0f, 1.0f, 0), 1.0f,
                   std::shared_ptr<MetalMaterial>(
                       new MetalMaterial(Vec3(0.7f, 0.6f, 0.5f), 0.0f)))));

    return world;
}

int main() {
    int nx = 640;
    int ny = 480;
    int ns = 100; // Number of samples to take for each pixel

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    const Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    const Vec3 horizontal(4.0f, 0.0f, 0.0f);
    const Vec3 vertical(0.0f, 2.0f, 0.0f);
    const Vec3 origin(0.0f, 0.0f, 0.0f);
    Vec3 lookFrom(13.0f, 2.0f, 3.0f);
    Vec3 lookAt(0.0f, 0.0f, 0.0f);
    // float distToFocus = vec3::length(lookFrom - lookAt);
    float distToFocus = 10.0f;
    float aperture    = 0.1f;
    Camera cam(lookFrom, lookAt, Vec3(0, 1, 0), degToRad(20.0f),
               float(nx) / float(ny), aperture, distToFocus);

    // std::shared_ptr<SurfaceList> world(new SurfaceList());

    // std::shared_ptr<LambertianMaterial> mat1(
    //     new LambertianMaterial(Vec3(0.1f, 0.2f, 0.5f)));
    // std::shared_ptr<LambertianMaterial> mat2(
    //     new LambertianMaterial(Vec3(0.8f, 0.8f, 0.0f)));
    // std::shared_ptr<MetalMaterial> mat3(
    //     new MetalMaterial(Vec3(0.8f, 0.6f, 0.2f), 0.0f));
    // std::shared_ptr<MetalMaterial> mat4(
    //     new MetalMaterial(Vec3(1.0f, 1.0f, 1.0f), 0.1f));
    // // std::shared_ptr<DielectricMaterial> mat4(new DielectricMaterial(1.5f));
    // std::shared_ptr<DielectricMaterial> mat5(new DielectricMaterial(1.5f));

    // world->addSurface(std::shared_ptr<Sphere>(
    //     new Sphere(Vec3(-0.5f, 0.0f, -1.0f), 0.5f, mat1)));
    // world->addSurface(std::shared_ptr<Sphere>(
    //     new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f, mat2)));
    // world->addSurface(std::shared_ptr<Sphere>(
    //     new Sphere(Vec3(0.5f, 0.0f, -1.0f), 0.5f, mat3)));
    // world->addSurface(std::shared_ptr<Sphere>(
    //     new Sphere(Vec3(0.0f, 0.0f, 0.0f), 0.5f, mat5)));
    // world->addSurface(std::shared_ptr<Sphere>(
    //     new Sphere(Vec3(0.0f, 0.0f, 0.0f), -0.45f, mat5)));


    std::shared_ptr<SurfaceList> world = getRandomScene();

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);
            // Sum samples
            for (int s = 0; s < ns; ++s) {
                float u = float(i + dist(mt)) / float(nx);
                float v = float(j + dist(mt)) / float(ny);
                Ray ray = cam.getRay(u, v);
                Vec3 p  = ray.pointAtParameter(2.0f);
                col += color(ray, world, 0);
            }

            // Average samples
            col /= float(ns);

            // Gamma correct colour
            col = gammaCorrect(col, 2.0f);

            int ir = floatToByte(col.x);
            int ig = floatToByte(col.y);
            int ib = floatToByte(col.z);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
