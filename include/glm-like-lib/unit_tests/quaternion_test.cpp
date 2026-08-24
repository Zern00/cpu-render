#include <gtest/gtest.h>
#include <glm-like-lib/quaternion.hpp>

TEST(quaternionTest, initAndOper) {
    gll::Quaternion i;

    EXPECT_EQ(i, gll::Quaternion(1, 0, 0, 0));

    gll::Quaternion q(M_PI / 2, gll::Vec3(0, 0, 2));

    EXPECT_EQ(q, gll::Quaternion(std::sqrt(2)/2, 0, 0, std::sqrt(2)/2));
    EXPECT_NE(q, gll::Quaternion(std::sqrt(2)/2, 0, 0, std::sqrt(2)));

    EXPECT_THROW(gll::Quaternion(M_PI, gll::Vec3(0, 0, 0)), std::runtime_error);
}

TEST(quaternionTest, axisNormalization) {
    gll::Vec3 axisNorm(1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0);
    gll::Vec3 axisNotNorm(1, 2, 2);

    gll::Quaternion q1(M_PI / 3, axisNorm);
    gll::Quaternion q2(M_PI / 3, axisNotNorm);

    gll::Quaternion expected(std::sqrt(3)/2, 1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0);

    EXPECT_EQ(q1, expected);
    EXPECT_EQ(q2, expected);
    EXPECT_EQ(q1, q2);
}

TEST(quaternionTest, inverseAndConjugated) {
    gll::Vec3 axisNorm(1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0);
    gll::Vec3 axisNotNorm(1, 2, 2);

    gll::Quaternion q1(M_PI / 3, axisNorm);
    gll::Quaternion q2(M_PI / 3, axisNotNorm);

    EXPECT_EQ(q1.inversed(), gll::Quaternion(M_PI / 3, -axisNorm));
    EXPECT_EQ(q2.inversed(), gll::Quaternion(M_PI / 3, -axisNorm));

    EXPECT_EQ(q1.inversed(), q1.conjugated());
    EXPECT_EQ(q2.inversed(), q2.conjugated());

    EXPECT_NE(q1.inversed(), -q1);

    gll::Quaternion q3 = q1 * 2;

    EXPECT_NE(q3.inversed(), q3.conjugated());
}

TEST(quaternionTest, rotate) {
    gll::Quaternion q(M_PI / 2, gll::Vec3(0, 0, 1));

    gll::Vec3 v1(1, 0, 0);
    gll::Vec3 v2(2, 0, 0);

    EXPECT_EQ(q.rotate(v1), gll::Vec3(0, 1, 0));
    EXPECT_EQ(q.rotate(v2), gll::Vec3(0, 2, 0));

    EXPECT_EQ(q.rotate(v2).length(), v2.length());

    gll::Vec3 axis(1, 2, 2);
    gll::Quaternion q2(M_PI / 3, axis);
    EXPECT_EQ(q2.rotate(axis), axis);
}
  

