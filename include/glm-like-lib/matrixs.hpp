#pragma once
#include <cstddef>
#include"vectors.hpp"
#include <glm-like-lib/Gfloat.hpp>

namespace gll {
    struct Mat3 {
        Gfloat data_[3][3]{};

        constexpr Mat3() = default;

        constexpr Mat3(Gfloat m00, Gfloat m01, Gfloat m02,
                       Gfloat m10, Gfloat m11, Gfloat m12,
                       Gfloat m20, Gfloat m21, Gfloat m22) noexcept {
            data_[0][0] = m00; data_[1][0] = m10; data_[2][0] = m20;
            data_[0][1] = m01; data_[1][1] = m11; data_[2][1] = m21;
            data_[0][2] = m02; data_[1][2] = m12; data_[2][2] = m22;
        }

        static constexpr Mat3 identity() noexcept {
            Mat3 res;
            for (int i = 0; i < 3; ++i)
                res.data_[i][i] = 1.0f;
            return res;
        }

        constexpr Gfloat* operator[](size_t col) noexcept {
            return data_[col];
        }
        constexpr const Gfloat* operator[](size_t col) const noexcept {
            return data_[col];
        }

        constexpr Mat3& operator*=(const Mat3& other) noexcept {
            Mat3 res{};
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    for (int k = 0; k < 3; ++k)
                        res[i][j] += data_[k][j] * other[i][k];
            *this = res;
            return *this;
        }

        constexpr Mat3& operator*=(Gfloat scal) noexcept {
            for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            data_[i][j] *= scal;
            return *this;
        }

        constexpr bool operator==(const Mat3& other) const noexcept {
            bool ok = true;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    ok *= (data_[i][j] == other[i][j]);
            return ok;
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

        friend constexpr Mat3 operator*(Mat3 mat, Gfloat scal) noexcept {
           mat *= scal;
           return mat;
        }

        friend constexpr Mat3 operator*(Gfloat scal, const Mat3& mat) noexcept {
            return mat * scal;
        }

        constexpr Mat3 transposed() const noexcept {
            Mat3 res{};
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    res[i][j] = data_[j][i];
            return res;
        }

        constexpr Gfloat determinant() const noexcept {
            Gfloat m00 = data_[0][0], m01 = data_[1][0], m02 = data_[2][0];
            Gfloat m10 = data_[0][1], m11 = data_[1][1], m12 = data_[2][1];
            Gfloat m20 = data_[0][2], m21 = data_[1][2], m22 = data_[2][2];

            return m00 * (m11 * m22 - m12 * m21)
                - m01 * (m10 * m22 - m12 * m20)
                + m02 * (m10 * m21 - m11 * m20);
        }

        Mat3 inverse() const;
    };

    struct Mat4 {
    public:
        Gfloat data_[4][4]{};

        constexpr Mat4() = default;

        constexpr Mat4(Gfloat m00, Gfloat m01, Gfloat m02, Gfloat m03,
                       Gfloat m10, Gfloat m11, Gfloat m12, Gfloat m13,
                       Gfloat m20, Gfloat m21, Gfloat m22, Gfloat m23,
                       Gfloat m30, Gfloat m31, Gfloat m32, Gfloat m33) noexcept {
            data_[0][0] = m00; data_[1][0] = m10; data_[2][0] = m20; data_[3][0] = m30;
            data_[0][1] = m01; data_[1][1] = m11; data_[2][1] = m21; data_[3][1] = m31;
            data_[0][2] = m02; data_[1][2] = m12; data_[2][2] = m22; data_[3][2] = m32;
            data_[0][3] = m03; data_[1][3] = m13; data_[2][3] = m23; data_[3][3] = m33;
        }

        static constexpr Mat4 identity() noexcept {
            Mat4 res;
            for (int i = 0; i < 4; ++i)
                res.data_[i][i] = 1.0f;
            return res;
        }

        constexpr Gfloat* operator[](size_t col) noexcept {
            return data_[col];
        }
        constexpr const Gfloat* operator[](size_t col) const noexcept {
            return data_[col];
        }

        constexpr Mat4& operator*=(const Mat4& other) noexcept {
            Mat4 res{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        res[i][j] += data_[k][j] * other[i][k];
            *this = res;
            return *this;
        }

        constexpr Mat4& operator*=(Gfloat scal) noexcept {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    data_[i][j] *= scal;
            return *this;
        }

        constexpr bool operator==(const Mat4& other) const noexcept {
            bool ok = true;
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    ok *= (data_[i][j] == other[i][j]);
            return ok;
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

        friend constexpr Mat4 operator*(Mat4 mat, Gfloat scal) noexcept {
           mat *= scal;
           return mat;
        }

        friend constexpr Mat4 operator*(Gfloat scal, const Mat4& mat) noexcept {
            return mat * scal;
        }

        constexpr Mat4 transposed() const noexcept {
            Mat4 res{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    res[i][j] = data_[j][i];
            return res;
        }

        constexpr Gfloat determinant() const noexcept {
            return data_[0][0] * minorDet3(0, 0) - data_[1][0] * minorDet3(0, 1)
                 + data_[2][0] * minorDet3(0, 2) - data_[3][0] * minorDet3(0, 3);
        }

        Mat4 inverse() const;
    private:
        constexpr Gfloat minorDet3(int exRow, int exCol) const noexcept {
            int r[3]{}, c[3]{};
            int idx = 0;
            for (int i = 0; i < 4; ++i) if (i != exRow) r[idx++] = i;
            idx = 0;
            for (int j = 0; j < 4; ++j) if (j != exCol) c[idx++] = j;

            Gfloat a = data_[c[0]][r[0]], b = data_[c[1]][r[0]], dc = data_[c[2]][r[0]];
            Gfloat d = data_[c[0]][r[1]], e = data_[c[1]][r[1]], f = data_[c[2]][r[1]];
            Gfloat g = data_[c[0]][r[2]], h = data_[c[1]][r[2]], i = data_[c[2]][r[2]];

            return a * (e * i - f * h) - b * (d * i - f * g) + dc * (d * h - e * g);
        }
    };
    
}
