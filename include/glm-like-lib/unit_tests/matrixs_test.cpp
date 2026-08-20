#include <gtest/gtest.h>
#include <glm-like-lib/matrixs.hpp>

TEST(Mat3Tests, IdentityDoesNotChangeMatrixOrVector) {
    const gll::Mat3 matrix(1, 2, 3,
                           4, 5, 6,
                           7, 8, 10);
    const gll::Vec3 vector(2, -3, 4);

    EXPECT_EQ(gll::Mat3::identity() * matrix, matrix);
    EXPECT_EQ(matrix * gll::Mat3::identity(), matrix);
    EXPECT_EQ(gll::Mat3::identity() * vector, vector);
}

TEST(Mat3Tests, DoubleTransposeReturnsOriginalMatrix) {
    const gll::Mat3 matrix(1, 2, 3,
                           4, 5, 6,
                           7, 8, 9);

    EXPECT_EQ(matrix.transposed().transposed(), matrix);
    EXPECT_EQ(matrix.transposed().determinant(), matrix.determinant());
}

TEST(Mat3Tests, InverseProducesIdentity) {
    const gll::Mat3 matrix(2, 0, 0,
                           0, 3, 0,
                           0, 0, 4);

    EXPECT_EQ(matrix.determinant(), 24);
    EXPECT_EQ(matrix * matrix.inverse(), gll::Mat3::identity());
    EXPECT_EQ(matrix.inverse() * matrix, gll::Mat3::identity());
}

TEST(Mat3Tests, SingularMatrixHasNoInverse) {
    EXPECT_THROW(gll::Mat3().inverse(), std::runtime_error);
}

TEST(Mat4Tests, IdentityDoesNotChangeMatrixOrVector) {
    const gll::Mat4 matrix(1, 2, 3, 4,
                           5, 6, 7, 8,
                           9, 10, 11, 12,
                           13, 14, 15, 16);
    const gll::Vec4 vector(2, -3, 4, 1);

    EXPECT_EQ(gll::Mat4::identity() * matrix, matrix);
    EXPECT_EQ(matrix * gll::Mat4::identity(), matrix);
    EXPECT_EQ(gll::Mat4::identity() * vector, vector);
}

TEST(Mat4Tests, DoubleTransposeReturnsOriginalMatrix) {
    const gll::Mat4 matrix(1, 2, 3, 4,
                           5, 6, 7, 8,
                           9, 10, 11, 12,
                           13, 14, 15, 16);

    EXPECT_EQ(matrix.transposed().transposed(), matrix);
    EXPECT_EQ(matrix.transposed().determinant(), matrix.determinant());
}

TEST(Mat4Tests, InverseProducesIdentity) {
    const gll::Mat4 matrix(2, -1,  3,  4,
                           0,  5,  2, -2,
                           1,  3, -1,  0,
                           4,  2,  1,  3);

    EXPECT_EQ(matrix.determinant(), 93.0f);
    EXPECT_EQ(matrix * matrix.inverse(), gll::Mat4::identity());
    EXPECT_EQ(matrix.inverse() * matrix, gll::Mat4::identity());
}

TEST(Mat4Tests, SingularMatrixHasNoInverse) {
    EXPECT_THROW(gll::Mat4().inverse(), std::runtime_error);
}
