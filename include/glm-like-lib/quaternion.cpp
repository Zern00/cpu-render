#include <glm-like-lib/quaternion.hpp>


namespace gll {
    Quaternion::Quaternion(Gfloat angle, const Vec3& axis) {
        Vec3 normAxis = axis.normalized();
        w = std::cos(angle/2);
        x = normAxis.x * sin(angle/2);
        y = normAxis.y * sin(angle/2);
        z = normAxis.z * sin(angle/2);
    }

    Gfloat Quaternion::length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    void Quaternion::normalize() {
        Gfloat lgh = this->length();
        if (lgh == 0.0f) {
            throw std::runtime_error("Cannot normalize quaternion with zero length");
        }
        w /= lgh;
        x /= lgh;
        y /= lgh;
        z /= lgh;
    }

    Quaternion Quaternion::normalized() const {
        Gfloat lgh = this->length();
        if (lgh == 0.0) { 
            throw std::runtime_error("Cannot return normalized quaternion with zero length"); 
        }
        return {w / lgh, x / lgh, y / lgh, z / lgh};
    }

    Quaternion Quaternion::inversed() const {
        Gfloat lgh = lengthSquared();
        if (lgh == 0.0f) {
            throw std::runtime_error("Cannot return inversed quaternion with zero length");
        }
        Quaternion c = conjugated();

        return {c.w / lgh, c.x / lgh, c.y / lgh, c.z / lgh};
    }

    Quaternion Quaternion::operator*(const Quaternion& q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y + y * q.w + z * q.x - x * q.z,
            w * q.z + z * q.w + x * q.y - y * q.x
        };
    }

    Vec3 Quaternion::rotate(const Vec3& v) const {
        Quaternion tmp{0, v.x, v.y, v.z};

        Quaternion res = (*this) * tmp * this->inversed();
        return {res.x, res.y, res.z};
    }

    Mat3 Quaternion::toMat3() const {
        const Quaternion q = normalized();

        const Gfloat xx = q.x * q.x;
        const Gfloat yy = q.y * q.y;
        const Gfloat zz = q.z * q.z;
        const Gfloat xy = q.x * q.y;
        const Gfloat xz = q.x * q.z;
        const Gfloat yz = q.y * q.z;
        const Gfloat wx = q.w * q.x;
        const Gfloat wy = q.w * q.y;
        const Gfloat wz = q.w * q.z;

        return {
            1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),        2.0f * (xz + wy),
            2.0f * (xy + wz),        1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),
            2.0f * (xz - wy),        2.0f * (yz + wx),        1.0f - 2.0f * (xx + yy)
        };
    }

    Mat4 Quaternion::toMat4() const {
        const Mat3 rotation = toMat3();
        return {
            rotation[0][0], rotation[1][0], rotation[2][0], 0.0f,
            rotation[0][1], rotation[1][1], rotation[2][1], 0.0f,
            rotation[0][2], rotation[1][2], rotation[2][2], 0.0f,
            0.0f,           0.0f,           0.0f,           1.0f
        };
    }
};
