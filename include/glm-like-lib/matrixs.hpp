#pragma once
#include <cstddef>
#include"vectors.hpp"

namespace gll {
    struct Mat3 {
        float data[3][3]{};

        constexpr Mat3() = default;

        constexpr Mat3(float m00, float m01, float m02,
                       float m10, float m11, float m12,
                       float m20, float m21, float m22) noexcept {
            data[0][0] = m00; data[1][0] = m10; data[2][0] = m20;
            data[0][1] = m01; data[1][1] = m11; data[2][1] = m21;
            data[0][2] = m02; data[1][2] = m12; data[2][2] = m22;
        }

        static constexpr Mat3 identity() noexcept {
            Mat3 res;
            for (int i = 0; i < 3; ++i)
                res.data[i][i] = 1.0f;
            return res;
        }

        constexpr float* operator[](size_t col) noexcept {
            return data[col];
        }
        constexpr const float* operator[](size_t col) const noexcept {
            return data[col];
        }

        constexpr Mat3& operator*=(const Mat3& other) noexcept {
            Mat3 res{};
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    for (int k = 0; k < 3; ++k)
                        res[i][j] += data[k][j] * other[i][k];
            *this = res;
            return *this;
        }

        constexpr Mat3& operator*=(float scal) noexcept {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    data[i][j] *= scal;
            return *this;
        }

        friend constexpr Mat3 operator*(const Mat3& left, const Mat3& right) noexcept {
            Mat3 res{};
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    for (int k = 0; k < 3; ++k)
                        res[i][j] += left[k][j] * right[i][k];
            return res;
        }

        friend constexpr Vec3 operator*(const Mat3& left, const Vec3& right) noexcept {
            return {
                left[0][0] * right.x + left[1][0] * right.y + left[2][0] * right.z,
                left[0][1] * right.x + left[1][1] * right.y + left[2][1] * right.z,
                left[0][2] * right.x + left[1][2] * right.y + left[2][2] * right.z
            };
        }

        friend constexpr Mat3 operator*(Mat3 mat, float scal) noexcept {
           mat *= scal;
           return mat;
        }

        friend constexpr Mat3 operator*(float scal, const Mat3& mat) noexcept {
            return mat * scal;
        }

        constexpr Mat3 transposed() const noexcept {
            Mat3 res{};
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    res[i][j] = data[j][i];
            return res;
        }

        constexpr float determinant() const noexcept {
            float m00 = data[0][0], m01 = data[1][0], m02 = data[2][0];
            float m10 = data[0][1], m11 = data[1][1], m12 = data[2][1];
            float m20 = data[0][2], m21 = data[1][2], m22 = data[2][2];

            return m00 * (m11 * m22 - m12 * m21)
                - m01 * (m10 * m22 - m12 * m20)
                + m02 * (m10 * m21 - m11 * m20);
        }

        Mat3 inverse() const {
            float det = determinant();
            if (det == 0.0f) {
                throw std::runtime_error("Singular matrix");
            }

            float invDet = 1.0f / det;

            float m00 = data[0][0], m01 = data[1][0], m02 = data[2][0];
            float m10 = data[0][1], m11 = data[1][1], m12 = data[2][1];
            float m20 = data[0][2], m21 = data[1][2], m22 = data[2][2];

            float c00 =  (m11 * m22 - m12 * m21);
            float c01 = -(m01 * m22 - m02 * m21);
            float c02 =  (m01 * m12 - m02 * m11);
            float c10 = -(m10 * m22 - m12 * m20);
            float c11 =  (m00 * m22 - m02 * m20);
            float c12 = -(m00 * m12 - m02 * m10);
            float c20 =  (m10 * m21 - m11 * m20);
            float c21 = -(m00 * m21 - m01 * m20);
            float c22 =  (m00 * m11 - m01 * m10);

            Mat3 res{};
            res[0][0] = c00 * invDet;
            res[1][0] = c10 * invDet;
            res[2][0] = c20 * invDet;
            res[0][1] = c01 * invDet;
            res[1][1] = c11 * invDet;
            res[2][1] = c21 * invDet; 
            res[0][2] = c02 * invDet;
            res[1][2] = c12 * invDet;
            res[2][2] = c22 * invDet;
            return res;
        }
    };

    struct Mat4 {
    public:
        float data[4][4]{};

        constexpr Mat4() = default;

        constexpr Mat4(float m00, float m01, float m02, float m03,
                       float m10, float m11, float m12, float m13,
                       float m20, float m21, float m22, float m23,
                       float m30, float m31, float m32, float m33) noexcept {
            data[0][0] = m00; data[1][0] = m10; data[2][0] = m20; data[3][0] = m30;
            data[0][1] = m01; data[1][1] = m11; data[2][1] = m21; data[3][1] = m31;
            data[0][2] = m02; data[1][2] = m12; data[2][2] = m22; data[3][2] = m32;
            data[0][3] = m03; data[1][3] = m13; data[2][3] = m23; data[3][3] = m33;
        }

        static constexpr Mat4 identity() noexcept {
            Mat4 res;
            for (int i = 0; i < 4; ++i)
                res.data[i][i] = 1.0f;
            return res;
        }

        constexpr float* operator[](size_t col) noexcept {
            return data[col];
        }
        constexpr const float* operator[](size_t col) const noexcept {
            return data[col];
        }

        constexpr Mat4& operator*=(const Mat4& other) noexcept {
            Mat4 res{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        res[i][j] += data[k][j] * other[i][k];
            *this = res;
            return *this;
        }

        constexpr Mat4& operator*=(float scal) noexcept {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    data[i][j] *= scal;
            return *this;
        }

        friend constexpr Mat4 operator*(const Mat4& left, const Mat4& right) noexcept {
            Mat4 res{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        res[i][j] += left[k][j] * right[i][k];
            return res;
        }

        friend constexpr Vec4 operator*(const Mat4& left, const Vec4& right) noexcept {
            return {
                left[0][0] * right.x + left[1][0] * right.y + left[2][0] * right.z + left[3][0] * right.w,
                left[0][1] * right.x + left[1][1] * right.y + left[2][1] * right.z + left[3][1] * right.w,
                left[0][2] * right.x + left[1][2] * right.y + left[2][2] * right.z + left[3][2] * right.w,
                left[0][3] * right.x + left[1][3] * right.y + left[2][3] * right.z + left[3][3] * right.w
            };
        }

        friend constexpr Mat4 operator*(Mat4 mat, float scal) noexcept {
           mat *= scal;
           return mat;
        }

        friend constexpr Mat4 operator*(float scal, const Mat4& mat) noexcept {
            return mat * scal;
        }

        constexpr Mat4 transposed() const noexcept {
            Mat4 res{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    res[i][j] = data[j][i];
            return res;
        }

        constexpr float determinant() const noexcept {
            return data[0][0] * minorDet3(0, 0) - data[1][0] * minorDet3(0, 1)
                 + data[2][0] * minorDet3(0, 2) - data[3][0] * minorDet3(0, 3);
        }

        Mat4 inverse() const {
            float det = determinant();
            if (det == 0.0f) {
                throw std::runtime_error("Singular matrix");
            }

            float invDet = 1.0f / det;
            Mat4 res;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    float cf = ((i + j) % 2 == 0 ? 1.0f : -1.0f) * minorDet3(j, i);
                    res[i][j] = cf * invDet;
                }
            }
            return res;
        }
    private:
        constexpr float minorDet3(int exRow, int exCol) const noexcept {
            int r[3]{}, c[3]{};
            int idx = 0;
            for (int i = 0; i < 4; ++i) if (i != exRow) r[idx++] = i;
            idx = 0;
            for (int j = 0; j < 4; ++j) if (j != exCol) c[idx++] = j;

            float a = data[c[0]][r[0]], b = data[c[1]][r[0]], dc = data[c[2]][r[0]];
            float d = data[c[0]][r[1]], e = data[c[1]][r[1]], f = data[c[2]][r[1]];
            float g = data[c[0]][r[2]], h = data[c[1]][r[2]], i = data[c[2]][r[2]];

            return a * (e * i - f * h) - b * (d * i - f * g) + dc * (d * h - e * g);
        }
    };
    
}