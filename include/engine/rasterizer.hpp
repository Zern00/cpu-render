#pragma once
#include "framebuffer.hpp"
#include "glm-like-lib/vectors.hpp"
#include "texture.hpp"

namespace egn {
    struct ShadedVertex {
        gll::Vec3 screenPos;
        gll::Vec3 viewPos;
        gll::Vec3 normal;
        gll::Vec2 uv;
        gll::Gfloat invW;
        uint32_t color;
    };

    struct Light {
        gll::Vec3 direction;
        gll::Gfloat ambient;
        gll::Gfloat diffuse;
        gll::Gfloat specular;
        gll::Gfloat shininess;
    };
    
    void drawTriangle(const egn::ShadedVertex& v0, const egn::ShadedVertex& v1, const egn::ShadedVertex& v2,
                      Framebuffer& fb, const Texture* tex = nullptr, const Light* light = nullptr);
}
