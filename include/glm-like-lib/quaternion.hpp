#pragma once
#include <glm-like-lib/gfloat.hpp>
#include <glm-like-lib/matrixs.hpp>
#include <cmath>

namespace gll {
struct Quaternion {
    Gfloat w = 1;
    Gfloat x = 0;
    Gfloat y = 0;
    Gfloat z = 0;

    constexpr Quaternion() = default;
    constexpr Quaternion(Gfloat w_, Gfloat x_, Gfloat y_, Gfloat z_) noexcept 
                        : w(w_), x(x_), y(y_), z(z_) {}

    
    Quaternion(Gfloat angle, const Vec3& axis);

    constexpr Gfloat lengthSquared() const {
        return w * w + x * x + y * y + z * z;
    }

    Gfloat length() const noexcept;

    void normalize();
    Quaternion normalized() const;

    Quaternion conjugated() const {
        return {w, -x, -y, -z};
    }
    
    Quaternion inversed() const;

    constexpr Gfloat dot(const Quaternion& q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    Quaternion operator*(const Quaternion& q) const;

    constexpr Quaternion& operator*=(Gfloat s) noexcept {
        w *= s;
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    constexpr Quaternion operator*(Gfloat s) const noexcept {
        return {w * s, x * s, y * s, z * s};
    }

    friend constexpr Quaternion operator*(Gfloat s, const Quaternion& q) noexcept {
        return q * s;
    }

    Quaternion operator-() const {
        return (*this) * -1;
    }

    constexpr bool operator<=>(const Quaternion& other) const noexcept = default;

    Vec3 rotate(const Vec3& v) const;

    Mat3 toMat3() const;

    Mat4 toMat4() const;
};
}
