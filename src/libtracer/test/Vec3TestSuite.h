#include <cmath>

#include <gtest/gtest.h>

#include <trc/Vec3.h>

TEST(Vec3, TestDefaultConstructor)
{
    trc::Vec3 vec = trc::Vec3();

    EXPECT_EQ(0.0f, vec.x);
    EXPECT_EQ(0.0f, vec.y);
    EXPECT_EQ(0.0f, vec.z);
}

TEST(Vec3, TestConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_EQ(x, vec.x);
    EXPECT_EQ(y, vec.y);
    EXPECT_EQ(z, vec.z);
}

TEST(Vec3, TestCopyConstructor)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec1 = trc::Vec3(x, y, z);
    trc::Vec3 vec2 = trc::Vec3(vec1);

    EXPECT_EQ(vec1.x, vec2.x);
    EXPECT_EQ(vec1.y, vec2.y);
    EXPECT_EQ(vec1.z, vec2.z);
}

TEST(Vec3, TestIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
}

TEST(Vec3, TestConstIndexOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    const trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_EQ(x, vec[0]);
    EXPECT_EQ(y, vec[1]);
    EXPECT_EQ(z, vec[2]);
}

TEST(Vec3, TestMultiplicationAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;
    float factor = -3.99f;

    trc::Vec3 vec = trc::Vec3(x, y, z);
    vec *= factor;

    EXPECT_EQ(x * factor, vec.x);
    EXPECT_EQ(y * factor, vec.y);
    EXPECT_EQ(z * factor, vec.z);
}

TEST(Vec3, TestAdditionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec1 = trc::Vec3(x, y, z);
    trc::Vec3 vec2 = trc::Vec3(x, y, z);
    vec1 += vec2;

    EXPECT_EQ(x * 2, vec1.x);
    EXPECT_EQ(y * 2, vec1.y);
    EXPECT_EQ(z * 2, vec1.z);
}

TEST(Vec3, TestSubtractionAssignmentOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec1 = trc::Vec3(x, y, z);
    trc::Vec3 vec2 = trc::Vec3(x, y, z);
    vec1 -= vec2;

    EXPECT_EQ(0.0f, vec1.x);
    EXPECT_EQ(0.0f, vec1.y);
    EXPECT_EQ(0.0f, vec1.z);
}

TEST(Vec3, TestEquivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec1 = trc::Vec3(x, y, z);
    trc::Vec3 vec2 = trc::Vec3(x, y, z);

    EXPECT_TRUE(vec1 == vec2);

    vec2.z = 0.3f;
    EXPECT_FALSE(vec1 == vec2);
}

TEST(Vec3, TestInequivalenceOperator)
{
    float x = 1.34f;
    float y = -10e6;
    float z = 10e-8;

    trc::Vec3 vec1 = trc::Vec3(x, y, z);
    trc::Vec3 vec2 = trc::Vec3(x, y, z);

    EXPECT_FALSE(vec1 != vec2);

    vec2.z = 0.3f;
    EXPECT_TRUE(vec1 != vec2);
}

TEST(Vec3, TestLength)
{
    float x = 2;
    float y = 2;
    float z = -99.0f;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_TRUE(fabs(trc::vec3::length(vec) -
                     sqrt((x * x) + (y * y) + (z * z))) <= trc::FLOAT_ACCURACY);
}

TEST(Vec3, TestNormalize)
{
    float x = 1.0f;
    float y = 0.0f;
    float z = 0.0f;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_EQ(trc::Vec3(x, y, z), trc::vec3::normalize(vec));

    x = 993.0f;
    y = 0.2f;
    z = -3.321f;

    vec = trc::Vec3(x, y, z);
    float mag = trc::vec3::length(vec);
    vec = trc::vec3::normalize(vec);

    EXPECT_EQ(trc::Vec3(x / mag, y / mag, z / mag), vec);
}

TEST(Vec3, TestInvert)
{
    float x = -13.0f;
    float y = 2.0f;
    float z = 0.0f;

    trc::Vec3 vec = trc::vec3::invert(trc::Vec3(x, y, z));

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);

    vec = -trc::Vec3(x, y, z);

    EXPECT_EQ(-x, vec.x);
    EXPECT_EQ(-y, vec.y);
    EXPECT_EQ(-z, vec.z);
}

TEST(Vec3, TestDotProduct)
{
    trc::Vec3 vec1 = trc::Vec3(3.0f, 1.0f, -23.03f);
    trc::Vec3 vec2 = trc::Vec3(-3.4f, 100.0f, -3.897f);
    float expectedResult = 179.54791f;

    EXPECT_EQ(expectedResult, trc::vec3::dot(vec1, vec2));
}

TEST(Vec3, TestCrossProduct)
{
    trc::Vec3 vec1 = trc::Vec3(3.0f, 1.0f, -23.03f);
    trc::Vec3 vec2 = trc::Vec3(-3.4f, 100.0f, -3.897f);
    trc::Vec3 cross = trc::vec3::cross(vec1, vec2);

    trc::Vec3 expectedResult = trc::Vec3((vec1.y * vec2.z) - (vec1.z * vec2.y),
                                       (vec1.z * vec2.x) - (vec1.x * vec2.z),
                                       (vec1.x * vec2.y) - (vec1.y * vec2.x));

    EXPECT_TRUE(expectedResult == cross);
}

TEST(Vec3, TestAdditionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;

    trc::Vec3 vec1 = trc::Vec3(x1, y1, z1);
    trc::Vec3 vec2 = trc::Vec3(x2, y2, z2);
    trc::Vec3 result = vec1 + vec2;

    EXPECT_EQ(x1 + x2, result.x);
    EXPECT_EQ(y1 + y2, result.y);
    EXPECT_EQ(z1 + z2, result.z);
}

TEST(Vec3, TestSubtractionOperator)
{
    float x1 = 3.0f;
    float y1 = -10e-4f;
    float z1 = 10e9f;
    float x2 = 0.039f;
    float y2 = 10e-9f;
    float z2 = -10e2f;

    trc::Vec3 vec1 = trc::Vec3(x1, y1, z1);
    trc::Vec3 vec2 = trc::Vec3(x2, y2, z2);
    trc::Vec3 result = vec1 - vec2;

    EXPECT_EQ(x1 - x2, result.x);
    EXPECT_EQ(y1 - y2, result.y);
    EXPECT_EQ(z1 - z2, result.z);
}

TEST(Vec3, TestScaleOperator)
{
    float x = 3.0f;
    float y = -10e-4f;
    float z = 10e9f;
    float scale = -9.4f;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    trc::Vec3 result = vec * scale;

    EXPECT_EQ(x * scale, result.x);
    EXPECT_EQ(y * scale, result.y);
    EXPECT_EQ(z * scale, result.z);
}

TEST(Vec3, TestOutputStreamOperator)
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
    std::stringstream stream, resultStream;
    trc::Vec3 vec = trc::Vec3(x, y, z);

    resultStream << x << " " << y << " " << z;
    stream << vec;

    EXPECT_EQ(resultStream.str(), stream.str());
}

TEST(Vec3, TestUnitX)
{
    EXPECT_EQ(trc::Vec3(1.0f, 0.0f, 0.0f), trc::vec3::unit_x);
}

TEST(Vec3, TestUnitY)
{
    EXPECT_EQ(trc::Vec3(0.0f, 1.0f, 0.0f), trc::vec3::unit_y);
}

TEST(Vec3, TestUnitZ)
{
    EXPECT_EQ(trc::Vec3(0.0f, 0.0f, 1.0f), trc::vec3::unit_z);
}

TEST(Vec3, TestSquaredLength)
{
    float x = 2;
    float y = 2;
    float z = -99.0f;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    EXPECT_TRUE(fabs(trc::vec3::squared_length(vec) -
                     ((x * x) + (y * y) + (z * z))) <= trc::FLOAT_ACCURACY);
}

TEST(Vec3, TestDivisionByFactor)
{
    float x = 3.0f;
    float y = -10e-4f;
    float z = 10e9f;
    float scale = -9.4f;

    trc::Vec3 vec = trc::Vec3(x, y, z);

    trc::Vec3 result = vec / scale;

    EXPECT_EQ(x / scale, result.x);
    EXPECT_EQ(y / scale, result.y);
    EXPECT_EQ(z / scale, result.z);

    vec /= scale;
    result = vec;

    EXPECT_EQ(x / scale, result.x);
    EXPECT_EQ(y / scale, result.y);
    EXPECT_EQ(z / scale, result.z);
}
