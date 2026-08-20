#include <gtest/gtest.h>
#include <glm-like-lib/transform.hpp>

TEST(translateTest, commonPath) {
    gll::Vec4 p{1, 2, 3, 1};
    gll::Vec4 v{1, 2, 3, 0};
    gll::Vec4 origin{0, 0, 0, 1};

    gll::Mat4 T = gll::translate(gll::Mat4::identity(), {2, 1, 3});

    EXPECT_EQ(T * p, gll::Vec4(3, 3, 6, 1));
    EXPECT_EQ(T * v, gll::Vec4(1, 2, 3, 0));
    EXPECT_EQ(T * origin, gll::Vec4(2, 1, 3, 1));
    EXPECT_EQ(T.inverse() * T * p, gll::Vec4(1, 2, 3, 1));
}

TEST(rotateTest, commonPath) {
    gll::Vec4 p{1, 2, 3, 1};

    gll::Mat4 R = gll::rotate(gll::Mat4::identity(), 1.57079633f, {1, 0, 0});

    EXPECT_EQ(R * p, gll::Vec4(1, -3, 2, 1));
    EXPECT_EQ(R.inverse() * R * p, gll::Vec4(1, 2, 3, 1));
}

TEST(scaleTest, commonPath) {
    gll::Vec4 p{2, 1, 1, 1};

    gll::Mat4 S = gll::scale(gll::Mat4::identity(), {0.5, 2, 0.5});

    EXPECT_EQ(S * p, gll::Vec4(1, 2, 0.5f, 1));
    EXPECT_EQ(S.inverse() * S * p, gll::Vec4(2, 1, 1, 1));

    gll::Mat4 S_singular = gll::scale(gll::Mat4::identity(), {0, 2, 0.5});
    EXPECT_EQ(S_singular.determinant(), 0);
}