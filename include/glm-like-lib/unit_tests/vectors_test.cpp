#include <gtest/gtest.h>
#include <glm-like-lib/vectors.hpp>

TEST(Vec2Tests, ArithmeticAndLenghthAreConsistent) {
    const gll::Vec2 left(3, 4);
    const gll::Vec2 right(4, 5);

    EXPECT_EQ(2 * (left + right), 2 * left + 2 * right);
    EXPECT_EQ((left + right) - right, left);
    EXPECT_EQ((left * 3) / 3, left);
    EXPECT_EQ(left + gll::Vec2(), left);
    EXPECT_EQ(-(-left), left);
    EXPECT_EQ(left.lengthSquared(), 25);
    EXPECT_EQ(left.length(), 5);
    EXPECT_EQ(left.length() * 2, (2 * left).length());
    EXPECT_EQ(left.normalized().length(), 1);
}

TEST(Vec2Tests, ZeroVectorCannotBeNormalized) {
    gll::Vec2 vector;

    EXPECT_THROW(vector.normalize(), std::runtime_error);
    EXPECT_THROW(vector.normalized(), std::runtime_error);
}

TEST(Vec3Tests, DotAndCrossProductsHaveCommonProperties) {
    const gll::Vec3 x(1, 0, 0);
    const gll::Vec3 y(0, 1, 0);
    const gll::Vec3 z(0, 0, 1);

    EXPECT_EQ(dot_product(x, y), 0);
    EXPECT_EQ(dot_product(x, x), x.lengthSquared());
    EXPECT_EQ(cross_product(x, y), z);
    EXPECT_EQ(cross_product(y, x), -z);
    EXPECT_EQ(dot_product(cross_product(x, y), x), 0);
    EXPECT_EQ(dot_product(cross_product(x, y), y), 0);
}

TEST(Vec3Tests, ZeroVectorCannotBeNormalized) {
    gll::Vec3 vector;

    EXPECT_THROW(vector.normalize(), std::runtime_error);
    EXPECT_THROW(vector.normalized(), std::runtime_error);
}

TEST(Vec4Tests, ArithmeticAndLengthAreConsistent) {
    const gll::Vec4 left(1, 2, 2, 4);
    const gll::Vec4 right(-2, 1, 3, 0);

    EXPECT_EQ(2 * (left + right), 2 * left + 2 * right);
    EXPECT_EQ(left.lengthSquared(), 25);
    EXPECT_EQ(left.length(), 5);
    EXPECT_EQ(left.length() * 2, (2 * left).length());
    EXPECT_EQ((left + right) - right, left);
    EXPECT_EQ((left * 2) / 2, left);
    EXPECT_EQ(left.normalized().length(), 1);
}

TEST(Vec4Tests, ConversionToAndFromVec3) {
    const gll::Vec3 vector(1, 2, 3);

    EXPECT_EQ(gll::Vec4::toVec4(vector, 2).toVec3(), vector / 2);
    EXPECT_THROW(gll::Vec4(1, 2, 3, 0).toVec3(), std::runtime_error);
}

TEST(Vec4Tests, ZeroVectorCannotBeNormalized) {
    gll::Vec4 vector;

    EXPECT_THROW(vector.normalize(), std::runtime_error);
    EXPECT_THROW(vector.normalized(), std::runtime_error);
}
