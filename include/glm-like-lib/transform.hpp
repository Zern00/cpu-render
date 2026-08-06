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

    Mat4 rotate(const Mat4& mat, float angle, const Vec3& vec) {
        Vec3 v = vec.normalized();
        float sin = std::sin(angle);
        float cos = std::cos(angle);

        Mat4 K{};
        K[0][0] = cos + v.x * v.x * (1.0f - cos);
        K[0][1] = v.y * v.x * (1.0f - cos) + v.z * sin;
        K[0][2] = v.z * v.x * (1.0f - cos) - v.y * sin;

        K[1][0] = v.x * v.y * (1.0f - cos) - v.z * sin;
        K[1][1] = cos + v.y * v.y * (1.0f - cos);
        K[1][2] = v.z * v.y * (1.0f - cos) + v.x * sin;

        K[2][0] = v.x * v.z * (1.0f - cos) + v.y * sin;
        K[2][1] = v.y * v.z * (1.0f - cos) - v.x * sin;
        K[2][2] = cos + v.z * v.z * (1.0f - cos);

        K[3][3] = 1;

        return mat * K;
    }

    constexpr Mat4 scale(const Mat4& mat, const Vec3& vec) noexcept {
        Mat4 res = Mat4::identity();
        res[0][0] *= vec.x;
        res[1][1] *= vec.y;
        res[2][2] *= vec.z;
        return mat * res;
    }

    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f = (center - eye).normalized();
        Vec3 s = cross_product(f, up.normalized()).normalized();
        Vec3 u = cross_product(s, f);

        Mat4 res{};
        res[0][0] = s.x;
        res[0][1] = u.x;
        res[0][2] = -f.x;

        res[1][0] = s.y;
        res[1][1] = u.y;
        res[1][2] = -f.y;

        res[2][0] = s.z;
        res[2][1] = u.z;
        res[2][2] = -f.z;

        res[3][0] = -dot_product(s, eye);
        res[3][1] = -dot_product(u, eye);
        res[3][2] = dot_product(f, eye);
        res[3][3] = 1;
        return res;
    }

    Mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
        float tan = std::tan(fovy * 0.5);
        Mat4 res{};

        res[0][0] = 1 / (aspect * tan);
        res[1][1] = 1 / tan;
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -1;
        res[3][2] = -(2 * zFar * zNear) / (zFar - zNear);

        return res;
    }

    constexpr Mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) noexcept {
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