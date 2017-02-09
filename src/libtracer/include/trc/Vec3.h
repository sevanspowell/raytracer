#pragma once

#include <iostream>

#include <trc/Precision.h>

namespace trc
{
class Vec3
{
public:
    explicit Vec3(scalar x_ = 0.0f, scalar y_ = 0.0f, scalar z_ = 0.0f)
        : x(x_), y(y_), z(z_)
    {
    }
    Vec3(const Vec3 &other) : x(other.x), y(other.y), z(other.z)
    {
    }
    const Vec3 &operator=(const Vec3 &other);

    scalar &operator[](unsigned int index);
    const scalar &operator[](unsigned int index) const;

    /// Component wise addition
    Vec3 &operator+=(const Vec3 &other);
    /// Component wise subtraction
    Vec3 &operator-=(const Vec3 &other);

    /// Component wise multiplication
    Vec3 &operator*=(const scalar scale);
    /// Component wise division
    Vec3 &operator/=(const scalar scale);

    bool operator==(const Vec3 &other) const;
    bool operator!=(const Vec3 &other) const;

    scalar x, y, z;
};

/// Component wise addition
Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
/// Component wise subtraction
Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
// Component-wise multiplication
Vec3 operator*(const Vec3 &vec1, const Vec3 &vec2);
// Component-wise multiplication
Vec3 operator*(const Vec3 &vec, scalar factor);
// Component-wise division
Vec3 operator/(const Vec3 &vec, scalar factor);
// Component-wise multiplication
Vec3 operator*(scalar factor, const Vec3 &vec);
// Component-wise division
Vec3 operator/(scalar factor, const Vec3 &vec);

// Invert vector
Vec3 operator-(const Vec3 &vec);

std::ostream &operator<<(std::ostream &out, const Vec3 &vec);
std::istream &operator>>(std::istream &in, Vec3 &vec);

namespace vec3
{
scalar dot(const Vec3 &v1, const Vec3 &v2);
scalar length(const Vec3 &vec);
scalar squared_length(const Vec3 &vec);
Vec3 cross(const Vec3 &v1, const Vec3 &v2);
Vec3 normalize(const Vec3 &vec);
Vec3 invert(const Vec3 &vec);
Vec3 multiply(const Vec3 &v1, const Vec3 &v2);

static const Vec3 unit_x = Vec3((scalar)1.0f, 0.0f, 0.0f);
static const Vec3 unit_y = Vec3(0.0f, (scalar)1.0f, 0.0f);
static const Vec3 unit_z = Vec3(0.0f, 0.0f, (scalar)1.0f);
}
}
