#pragma once
#include "concurrency.hpp"
#include "rasterizer.hpp"
#include "glm-like-lib/vectors.hpp"
#include "glm-like-lib/transform.hpp"
#include "glm-like-lib/matrixs.hpp"
#include "mesh.hpp"

namespace egn {
    struct ClipVertex {
        gll::Vec4 clipPos;
        gll::Vec3 viewPos;
        gll::Vec3 normal;
        gll::Vec2 uv;
        gll::Vec4 color;
    };

    enum class ClipPlane {
        Left,
        Right,
        Bottom,
        Top,
        Near,
        Far
    };

    gll::Vec4 unpackColor(uint32_t color);

    gll::Gfloat boundary(const ClipVertex& v, ClipPlane plane);

    std::vector<ClipVertex> clippingVsPlane(const std::vector<ClipVertex>& p, ClipPlane plane);

    egn::ShadedVertex toShadedVertex(const ClipVertex& v, Framebuffer& fb);

    void drawMesh(const Mesh& mesh, const gll::Mat4& model, const gll::Mat4& view, const gll::Mat4& proj,
                  Framebuffer& fb, Threadpool& threadpool, const Texture* tex = nullptr, const Light* light = nullptr);
}
