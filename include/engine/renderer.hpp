#pragma once
#include "rasterizer.hpp"
#include "glm-like-lib/vectors.hpp"
#include "glm-like-lib/transform.hpp"
#include "glm-like-lib/matrixs.hpp"
#include "mesh.hpp"

namespace egn {
    void drawMesh(const Mesh& mesh, const gll::Mat4& model, const gll::Mat4& view, const gll::Mat4& proj,
                  Framebuffer& fb, const Texture* tex = nullptr, const Light* light = nullptr);
}
