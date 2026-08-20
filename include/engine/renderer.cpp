#include "renderer.hpp"

namespace egn {
    void drawMesh(const Mesh& mesh, const gll::Mat4& model, const gll::Mat4& view, const gll::Mat4& proj,
                  Framebuffer& fb, const Texture* tex, const Light* light) {
        gll::Mat4 mvp = proj * view * model;
        gll::Mat4 mv = view * model;

        gll::Vec3 lightDirView;
        if (light) {
            gll::Vec4 lightDir4 = view * gll::Vec4(light->direction.x, light->direction.y, light->direction.z, 0.0f);
            lightDirView = gll::Vec3(lightDir4.x, lightDir4.y, lightDir4.z).normalized();
        }
        
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            const Vertex& v0 = mesh.vertices[mesh.indices[i]];
            const Vertex& v1 = mesh.vertices[mesh.indices[i + 1]];
            const Vertex& v2 = mesh.vertices[mesh.indices[i + 2]];

            gll::Vec4 view0 = mv * gll::Vec4(v0.pos.x, v0.pos.y, v0.pos.z, 1.0f);
            gll::Vec4 view1 = mv * gll::Vec4(v1.pos.x, v1.pos.y, v1.pos.z, 1.0f);
            gll::Vec4 view2 = mv * gll::Vec4(v2.pos.x, v2.pos.y, v2.pos.z, 1.0f);

            gll::Mat4 normal_mv = mv.inverse().transposed();
            gll::Vec4 norm0 = normal_mv * gll::Vec4(v0.normal.x, v0.normal.y, v0.normal.z, 0.0f);
            gll::Vec4 norm1 = normal_mv * gll::Vec4(v1.normal.x, v1.normal.y, v1.normal.z, 0.0f);
            gll::Vec4 norm2 = normal_mv * gll::Vec4(v2.normal.x, v2.normal.y, v2.normal.z, 0.0f);

            norm0.normalize();
            norm1.normalize();
            norm2.normalize();

            gll::Vec4 clip0 = mvp * gll::Vec4(v0.pos.x, v0.pos.y, v0.pos.z, 1.0f);
            gll::Vec4 clip1 = mvp * gll::Vec4(v1.pos.x, v1.pos.y, v1.pos.z, 1.0f);
            gll::Vec4 clip2 = mvp * gll::Vec4(v2.pos.x, v2.pos.y, v2.pos.z, 1.0f);

            if (clip0.w <= gll::Gfloat::EPSILON || clip1.w <= gll::Gfloat::EPSILON || clip2.w <= gll::Gfloat::EPSILON) {
                continue;;
            }

            gll::Vec3 ndc0 = clip0.toVec3();
            gll::Vec3 ndc1 = clip1.toVec3();
            gll::Vec3 ndc2 = clip2.toVec3();
            
            egn::ShadedVertex sv0, sv1, sv2;

            sv0.screenPos = gll::Vec3((ndc0.x * 0.5f + 0.5f) * fb.width(), (1.0f - (ndc0.y * 0.5f + 0.5f)) * fb.height(), ndc0.z);
            sv1.screenPos = gll::Vec3((ndc1.x * 0.5f + 0.5f) * fb.width(), (1.0f - (ndc1.y * 0.5f + 0.5f)) * fb.height(), ndc1.z);
            sv2.screenPos = gll::Vec3((ndc2.x * 0.5f + 0.5f) * fb.width(), (1.0f - (ndc2.y * 0.5f + 0.5f)) * fb.height(), ndc2.z);

            sv0.invW = 1.0f / clip0.w;
            sv1.invW = 1.0f / clip1.w;
            sv2.invW = 1.0f / clip2.w;

            auto packColorOverW = [](uint32_t color, gll::Gfloat invW) -> uint32_t {
                uint8_t r = (color) & 0xFF, g = (color >> 8) & 0xFF, b = (color >> 16) & 0xFF;
                r = static_cast<uint8_t>(r * invW);
                g = static_cast<uint8_t>(g * invW);
                b = static_cast<uint8_t>(b * invW);
                return Framebuffer::packColor(r, g, b);
            };
            
            sv0.color = packColorOverW(v0.color, sv0.invW);
            sv1.color = packColorOverW(v1.color, sv1.invW);
            sv2.color = packColorOverW(v2.color, sv2.invW);

            sv0.uv = gll::Vec2(v0.uv.x * sv0.invW, v0.uv.y * sv0.invW);
            sv1.uv = gll::Vec2(v1.uv.x * sv1.invW, v1.uv.y * sv1.invW);
            sv2.uv = gll::Vec2(v2.uv.x * sv2.invW, v2.uv.y * sv2.invW);

            sv0.viewPos = gll::Vec3(view0.x * sv0.invW, view0.y * sv0.invW, view0.z * sv0.invW);
            sv1.viewPos = gll::Vec3(view1.x * sv1.invW, view1.y * sv1.invW, view1.z * sv1.invW);
            sv2.viewPos = gll::Vec3(view2.x * sv2.invW, view2.y * sv2.invW, view2.z * sv2.invW);

            sv0.normal = gll::Vec3(norm0.x * sv0.invW, norm0.y * sv0.invW, norm0.z * sv0.invW);
            sv1.normal = gll::Vec3(norm1.x * sv1.invW, norm1.y * sv1.invW, norm1.z * sv1.invW);
            sv2.normal = gll::Vec3(norm2.x * sv2.invW, norm2.y * sv2.invW, norm2.z * sv2.invW);

            Light viewLight;
            if (light) {
                viewLight = *light;
                viewLight.direction = lightDirView;
            }

            drawTriangle(sv0, sv1, sv2, fb, tex, light ? &viewLight : nullptr);
        }
    }
}
