#include "vectors.hpp"

namespace gll {
    Gfloat Vec2::length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    void Vec2::normalize() {
        Gfloat lgh = this->length();
        if (lgh == 0.0f) {
            throw std::runtime_error("Cannot normalize vec2 with zero length");
        }
        x /= lgh;
        y /= lgh;
    }

    Vec2 Vec2::normalized() const {
        Gfloat lgh = this->length();
        if (lgh == 0.0) { 
            throw std::runtime_error("Cannot return normalized vec2 with zero length"); 
        }
        return {x / lgh, y / lgh};
    }

    Gfloat Vec3::length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    void Vec3::normalize() {
        Gfloat lgh = this->length();
        if (lgh == 0.0f) {
            throw std::runtime_error("Cannot normalize vec3 with zero length");
        }
        x /= lgh;
        y /= lgh;
        z /= lgh;
    }

    Vec3 Vec3::normalized() const {
        Gfloat lgh = this->length();
        if (lgh == 0.0) { 
            throw std::runtime_error("Cannot return normalized vec3 with zero length"); 
        }
        return {x / lgh, y / lgh, z / lgh};
    }

    Gfloat Vec4::length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    void Vec4::normalize() {
        Gfloat lgh = this->length();
        if (lgh == 0.0f) {
            throw std::runtime_error("Cannot normalize vec4 with zero length");
        }
        x /= lgh;
        y /= lgh;
        z /= lgh;
        w /= lgh;
    }

    Vec4 Vec4::normalized() const {
        Gfloat lgh = this->length();
        if (lgh == 0.0) { 
            throw std::runtime_error("Cannot return normalized vec4 with zero length"); 
        }
        return {x / lgh, y / lgh, z / lgh, w / lgh};
    }

    Vec3 Vec4::toVec3() const {
        if (w == 0.0f) {
            throw std::runtime_error("Cannot convert Vec4 to Vec3: w is 0");
        }
        return {x / w, y / w, z / w};
    }
}
