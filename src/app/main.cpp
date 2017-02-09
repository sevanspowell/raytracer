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

trc::Vec3 color(const trc::Ray &ray)
{
    trc::Vec3 unitDir = trc::vec3::normalize(ray.direction());
    // Convert -1.0 < y < 1.0 to 0.0 < t < 1.0
    float t = 0.5 * (unitDir.y + 1.0);
    // Linearly blend white and blue based on y-parameter t
    // Linear interpolation formula = (1 - t) * start_value + t * end_value
    return (1.0 - t) * trc::Vec3(1.0f, 1.0f, 1.0f) +
           t * trc::Vec3(0.5f, 0.7f, 1.0f);
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
            trc::Vec3 col = color(ray);
            int ir = floatToByte(col.x);
            int ig = floatToByte(col.y);
            int ib = floatToByte(col.z);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
