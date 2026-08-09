#pragma once
#include "vectors.hpp"
#include "matrixs.hpp"

namespace gll {
    constexpr Mat4 translate(const Mat4& mat, const Vec3& vec) noexcept {
        Mat4 res = Mat4::identity();
        res[3][0] = vec.x;
        res[3][1] = vec.y;
        res[3][2] = vec.z;
        return mat * res;
    }

    Mat4 rotate(const Mat4& mat, Gfloat angle, const Vec3& vec);

    constexpr Mat4 scale(const Mat4& mat, const Vec3& vec) noexcept {
        Mat4 res = Mat4::identity();
        res[0][0] *= vec.x;
        res[1][1] *= vec.y;
        res[2][2] *= vec.z;
        return mat * res;
    }

    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

    Mat4 perspective(Gfloat fovy, Gfloat aspect, Gfloat zNear, Gfloat zFar);

    constexpr Mat4 ortho(Gfloat left, Gfloat right, Gfloat bottom, Gfloat top, Gfloat zNear, Gfloat zFar) noexcept {
        Mat4 res = Mat4::identity();

        res[0][0] = 2 / (right - left);
        res[1][1] = 2 / (top - bottom);
        res[2][2] = -2 / (zFar - zNear);
        res[3][0] = - (right + left) / (right - left);
		res[3][1] = - (top + bottom) / (top - bottom);
		res[3][2] = - (zFar + zNear) / (zFar - zNear);

        return res;
    }
}
