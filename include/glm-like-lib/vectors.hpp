#pragma once
#include <cmath>
#include <stdexcept>
#include "Gfloat.hpp"

namespace gll {
    struct Vec2 {
        Gfloat x = 0.0f, y = 0.0f;

        constexpr Vec2(Gfloat x_, Gfloat y_) noexcept : x(x_), y(y_) {}
        constexpr Vec2() = default;

        constexpr Vec2 operator-() const noexcept {
            return {-x, -y};
        }

        constexpr Vec2& operator+=(const Vec2& other) noexcept {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vec2& operator-=(const Vec2& other) noexcept {
            *this += -other;
            return *this;
        }

        constexpr Vec2& operator*=(Gfloat scal) noexcept {
            x *= scal;
            y *= scal;
            return *this;
        }

        constexpr Vec2& operator/=(Gfloat scal) noexcept {
            x /= scal;
            y /= scal;
            return *this;
        }

        friend constexpr Vec2 operator+(Vec2 left, const Vec2& right) noexcept {
            left += right;
            return left;
        }

        friend constexpr Vec2 operator-(Vec2 left, const Vec2& right) noexcept {
            left -= right;
            return left;
        }

        friend constexpr Vec2 operator*(Vec2 vec, Gfloat scal) noexcept {
            vec *= scal;
            return vec;
        }

        friend constexpr Vec2 operator*(Gfloat scal, Vec2 vec) noexcept {
            return vec * scal;
        }

        friend constexpr Vec2 operator/(Vec2 vec, Gfloat scal) noexcept {
            vec /= scal;
            return vec;
        }

        constexpr bool operator==(const Vec2& other) const noexcept {
            return (this->x == other.x) && (this->y == other.y);
        }

        constexpr bool operator!=(const Vec2& other) const noexcept {
            return !(*this == other);
        }

        friend constexpr Gfloat dot_product(const Vec2& left, const Vec2& right) noexcept {
            return (left.x * right.x) + (left.y * right.y);
        }

        constexpr Gfloat lengthSquared() const noexcept {
            return dot_product(*this, *this);
        }

        Gfloat length() const noexcept;

        void normalize();

        Vec2 normalized() const;
    };
    
    struct Vec3 {
        Gfloat x = 0.0f, y = 0.0f, z = 0.0f;

        constexpr Vec3(Gfloat x_, Gfloat y_, Gfloat z_) noexcept : x(x_), y(y_), z(z_) {}
        constexpr Vec3() = default;

        constexpr Vec3 operator-() const noexcept {
            return {-x, -y, -z};
        }

        constexpr Vec3& operator+=(const Vec3& other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vec3& operator-=(const Vec3& other) noexcept {
            *this += -other;
            return *this;
        }

        constexpr Vec3& operator*=(Gfloat scal) noexcept {
            x *= scal;
            y *= scal;
            z *= scal;
            return *this;
        }

        constexpr Vec3& operator/=(Gfloat scal) noexcept {
            x /= scal;
            y /= scal;
            z /= scal;
            return *this;
        }

        constexpr friend Vec3 operator+(Vec3 left, const Vec3& right) noexcept {
            left += right;
            return left;
        }

        constexpr friend Vec3 operator-(Vec3 left, const Vec3& right) noexcept {
            left -= right;
            return left;
        }

        constexpr friend Vec3 operator*(Vec3 vec, Gfloat scal) noexcept {
            vec *= scal;
            return vec;
        }

        constexpr friend Vec3 operator*(Gfloat scal, Vec3 vec) noexcept {
            return vec * scal;
        }

        friend constexpr Vec3 operator/(Vec3 vec, Gfloat scal) noexcept {
            vec /= scal;
            return vec;
        }

        constexpr bool operator==(const Vec3& other) const noexcept {
            return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
        }

        constexpr bool operator!=(const Vec3& other) const noexcept {
            return !(*this == other);
        }

        friend constexpr Gfloat dot_product(const Vec3& left, const Vec3& right) noexcept {
            return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
        }

        constexpr Gfloat lengthSquared() const noexcept {
            return dot_product(*this, *this);
        }

        Gfloat length() const noexcept;

        void normalize();

        Vec3 normalized() const;

        friend constexpr Vec3 cross_product(const Vec3& left, const Vec3& right) noexcept {
            return {
                left.y * right.z - left.z * right.y,
                left.z * right.x - left.x * right.z,
                left.x * right.y - left.y * right.x
            };
        }

        //lightDir should be normalized
        friend constexpr Vec3 reflect(const Vec3& lightDir, const Vec3& normal) noexcept {
            return lightDir - 2 * dot_product(lightDir, normal) * normal;
        }
    };

    struct Vec4 {
        Gfloat x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

        constexpr Vec4(Gfloat x_, Gfloat y_, Gfloat z_, Gfloat w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}
        constexpr Vec4() = default;

        constexpr Vec4 operator-() const noexcept {
            return {-x, -y, -z, -w};
        }

        constexpr Vec4& operator+=(const Vec4& other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        constexpr Vec4& operator-=(const Vec4& other) noexcept {
            *this += -other;
            return *this;
        }

        constexpr Vec4& operator*=(Gfloat scal) noexcept {
            x *= scal;
            y *= scal;
            z *= scal;
            w *= scal;
            return *this;
        }

        constexpr Vec4& operator/=(Gfloat scal) noexcept {
            x /= scal;
            y /= scal;
            z /= scal;
            w /= scal;
            return *this;
        }

        friend constexpr Vec4 operator+(Vec4 left, const Vec4& right) noexcept {
            left += right;
            return left;
        }

        friend constexpr Vec4 operator-(Vec4 left, const Vec4& right) noexcept {
            left -= right;
            return left;
        }

        friend constexpr Vec4 operator*(Vec4 vec, Gfloat scal) noexcept {
            vec *= scal;
            return vec;
        }

        friend constexpr Vec4 operator*(Gfloat scal, Vec4 vec) noexcept {
            return vec * scal;
        }

        friend constexpr Vec4 operator/(Vec4 vec, Gfloat scal) noexcept {
            vec /= scal;
            return vec;
        }

        constexpr bool operator==(const Vec4& other) const noexcept {
            return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
        }

        constexpr bool operator!=(const Vec4& other) const noexcept {
            return !(*this == other);
        }

        friend constexpr Gfloat dot_product(const Vec4& left, const Vec4& right) noexcept {
            return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
        }

        constexpr Gfloat lengthSquared() const noexcept {
            return dot_product(*this, *this);
        }

        Gfloat length() const noexcept;

        void normalize();

        Vec4 normalized() const;

        Vec3 toVec3() const;

        static constexpr Vec4 toVec4(const Vec3& vec, Gfloat scal) noexcept {
            return {vec.x, vec.y, vec.z, scal};
        }
    };
}
