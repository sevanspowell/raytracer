#include <cassert>
#include <cmath>

#include <trc/Vec3.h>

namespace trc
{
const Vec3 &Vec3::operator=(const Vec3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return (*this);
}

scalar &Vec3::operator[](unsigned int index)
{
    assert(index < 3);

    switch (index) {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    default:
        return x;
        break;
    }
}

const scalar &Vec3::operator[](unsigned int index) const
{
    assert(index < 3);

    switch (index) {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    default:
        return x;
        break;
    }
}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return (*this);
}
Vec3 &Vec3::operator-=(const Vec3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return (*this);
}

Vec3 &Vec3::operator*=(const scalar scale)
{
    x *= scale;
    y *= scale;
    z *= scale;

    return (*this);
}
Vec3 &Vec3::operator/=(const scalar scale)
{
    x /= scale;
    y /= scale;
    z /= scale;

    return (*this);
}

bool Vec3::operator==(const Vec3 &other) const
{
    return (fabs(x - other.x) < FLOAT_ACCURACY &&
            fabs(y - other.y) < FLOAT_ACCURACY &&
            fabs(z - other.z) < FLOAT_ACCURACY);
}
bool Vec3::operator!=(const Vec3 &other) const
{
    return (!(*this == other));
}

Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return (Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}
Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return (Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

Vec3 operator*(const Vec3 &vec, scalar factor)
{
    Vec3 scaled = vec;

    scaled *= factor;

    return scaled;
}
Vec3 operator/(const Vec3 &vec, scalar factor)
{
    Vec3 scaled = vec;

    scaled /= factor;

    return scaled;
}
Vec3 operator*(scalar factor, const Vec3 &vec)
{
    return vec * factor;
}
Vec3 operator/(scalar factor, const Vec3 &vec)
{
    return vec / factor;
}

Vec3 operator-(const Vec3 &vec)
{
    return vec3::invert(vec);
}

std::ostream &operator<<(std::ostream &out, const Vec3 &vec)
{
    out << vec.x << " " << vec.y << " " << vec.z;

    return out;
}

std::istream &operator>>(std::istream &in, Vec3 &vec)
{
    in >> vec[0] >> vec[1] >> vec[2];
    return in;
}

namespace vec3
{
scalar dot(const Vec3 &v1, const Vec3 &v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

scalar length(const Vec3 &vec)
{
    return sqrt(dot(vec, vec));
}

scalar squared_length(const Vec3 &vec)
{
    return dot(vec, vec);
}

Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return (Vec3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z),
                 (v1.x * v2.y) - (v1.y * v2.x)));
}

Vec3 normalize(const Vec3 &vec)
{
    scalar magnitude = length(vec);

    assert(magnitude != 0);

    return (vec * (((scalar)1.0f) / magnitude));
}

Vec3 invert(const Vec3 &vec)
{
    return (vec * -1);
}

Vec3 multiply(const Vec3 &v1, const Vec3 &v2)
{
    Vec3 vec;

    vec.x = v1.x * v2.x;
    vec.y = v1.y * v2.y;
    vec.z = v1.z * v2.z;

    return vec;
}

Vec3 divide(const Vec3 &v1, const Vec3 &v2)
{
    Vec3 vec;

    vec.x = v1.x / v2.x;
    vec.y = v1.y / v2.y;
    vec.z = v1.z / v2.z;

    return vec;
}
}
}
