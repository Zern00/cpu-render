#include "renderer.hpp"
#include <thread>

namespace egn {
    gll::Vec4 unpackColor(uint32_t color) {
        return gll::Vec4((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF);
    }

    gll::Gfloat boundary(const ClipVertex& v, ClipPlane plane) {
        const gll::Vec4& p = v.clipPos;
        
        switch (plane) {
            case ClipPlane::Left:
                return p.w + p.x;
            case ClipPlane::Right:
                return p.w - p.x;
            case ClipPlane::Bottom:
                return p.w + p.y;
            case ClipPlane::Top:
                return p.w - p.y;
            case ClipPlane::Near:
                return p.w + p.z;
            case ClipPlane::Far:
                return p.w - p.z;
        }

        return 0;
    }

    std::vector<ClipVertex> clippingVsPlane(const std::vector<ClipVertex>& p, ClipPlane plane) {
        std::vector<ClipVertex> o;

        if (p.empty()) {
            return o;
        }
        ClipVertex last = *p.rbegin();
        gll::Gfloat BClast = boundary(last, plane);
        bool insideLast = BClast >= 0;

        for (const ClipVertex& cur : p) {
            gll::Gfloat BCcur = boundary(cur, plane);
            bool insideCur = BCcur >= 0;

            if (insideLast && insideCur) {
                o.push_back(cur);
            } else if (!insideCur && !insideLast) {
                //nothing
            } else {
                gll::Gfloat t = BClast / (BClast - BCcur);

                ClipVertex r;
                r.clipPos = last.clipPos + (cur.clipPos - last.clipPos) * t;
                r.viewPos = last.viewPos + (cur.viewPos - last.viewPos) * t;
                r.normal = last.normal + (cur.normal - last.normal) * t;
                r.uv = last.uv + (cur.uv - last.uv) * t;
                r.color = last.color + (cur.color - last.color) * t;

                o.push_back(r);
                if (!insideLast && insideCur) {
                    o.push_back(cur);
                }
            }
            
            last = cur;
            BClast = BCcur;
            insideLast = insideCur;
        }
        return o;
    }

    egn::ShadedVertex toShadedVertex(const ClipVertex& v, Framebuffer& fb) {
        egn::ShadedVertex out;
        
        gll::Gfloat invW = 1.0f / v.clipPos.w;
        gll::Vec3 ndc(v.clipPos.x * invW, v.clipPos.y * invW, v.clipPos.z * invW);

        out.screenPos = {(ndc.x * 0.5f + 0.5f) * fb.width(), (0.5f - ndc.y * 0.5f) * fb.height(), ndc.z};

        out.invW = invW;
        out.uv = v.uv * invW;
        out.viewPos = v.viewPos * invW;
        out.normal = v.normal * invW;      
        out.color = v.color * invW;
        return out;
    }

    void drawMesh(const Mesh& mesh, const gll::Mat4& model, const gll::Mat4& view, const gll::Mat4& proj,
                  Framebuffer& fb, const Texture* tex, const Light* light) {
        gll::Mat4 mvp = proj * view * model;
        gll::Mat4 mv = view * model;

        gll::Vec3 lightDirView;
        if (light) {
            gll::Vec4 lightDir4 = view * gll::Vec4(light->direction.x, light->direction.y, light->direction.z, 0.0f);
            lightDirView = gll::Vec3(lightDir4.x, lightDir4.y, lightDir4.z).normalized();
        }
        
        gll::Mat4 normal_mv = mv.inverse().transposed();
        std::vector<std::vector<ClipVertex>> prepared_polygons;

        for (int i = 0; i < mesh.indices.size(); i += 3) {
            const Vertex& v0 = mesh.vertices[mesh.indices[i]];
            const Vertex& v1 = mesh.vertices[mesh.indices[i + 1]];
            const Vertex& v2 = mesh.vertices[mesh.indices[i + 2]];

            gll::Vec4 view0 = mv * gll::Vec4(v0.pos.x, v0.pos.y, v0.pos.z, 1.0f);
            gll::Vec4 view1 = mv * gll::Vec4(v1.pos.x, v1.pos.y, v1.pos.z, 1.0f);
            gll::Vec4 view2 = mv * gll::Vec4(v2.pos.x, v2.pos.y, v2.pos.z, 1.0f);

            gll::Vec4 norm0 = normal_mv * gll::Vec4(v0.normal.x, v0.normal.y, v0.normal.z, 0.0f);
            gll::Vec4 norm1 = normal_mv * gll::Vec4(v1.normal.x, v1.normal.y, v1.normal.z, 0.0f);
            gll::Vec4 norm2 = normal_mv * gll::Vec4(v2.normal.x, v2.normal.y, v2.normal.z, 0.0f);

            norm0.normalize();
            norm1.normalize();
            norm2.normalize();

            ClipVertex cv0;
            cv0.clipPos = mvp * gll::Vec4(v0.pos.x, v0.pos.y, v0.pos.z, 1.0f);
            cv0.viewPos = {view0.x, view0.y, view0.z};
            cv0.normal = {norm0.x, norm0.y, norm0.z};
            cv0.uv = v0.uv;
            cv0.color = unpackColor(v0.color);
            ClipVertex cv1;
            cv1.clipPos = mvp * gll::Vec4(v1.pos.x, v1.pos.y, v1.pos.z, 1.0f);
            cv1.viewPos = {view1.x, view1.y, view1.z};
            cv1.normal = {norm1.x, norm1.y, norm1.z};
            cv1.uv = v1.uv;
            cv1.color = unpackColor(v1.color);
            ClipVertex cv2;
            cv2.clipPos = mvp * gll::Vec4(v2.pos.x, v2.pos.y, v2.pos.z, 1.0f);
            cv2.viewPos = {view2.x, view2.y, view2.z};
            cv2.normal = {norm2.x, norm2.y, norm2.z};
            cv2.uv = v2.uv;
            cv2.color = unpackColor(v2.color);

            std::vector<ClipVertex> polygon{cv0, cv1, cv2};
            //clipping
            polygon = clippingVsPlane(polygon, ClipPlane::Left);
            polygon = clippingVsPlane(polygon, ClipPlane::Right);
            polygon = clippingVsPlane(polygon, ClipPlane::Bottom);
            polygon = clippingVsPlane(polygon, ClipPlane::Top);
            polygon = clippingVsPlane(polygon, ClipPlane::Near);
            polygon = clippingVsPlane(polygon, ClipPlane::Far);

            if (polygon.size() < 3) {
                continue;
            }

            prepared_polygons.push_back(polygon);
        }

        Light viewLight;
        if (light) {
            viewLight = *light;
            viewLight.direction = lightDirView;
        }

        uint32_t workers_count = std::max(std::thread::hardware_concurrency() - 2, 1U);
        std::vector<std::jthread> workers;
        workers.reserve(workers_count);

        for (size_t worker = 0; worker < workers_count; ++worker) {
            uint32_t startRow = (fb.height() * worker / workers_count) + static_cast<uint32_t>(worker > 0);
            uint32_t endRow = (fb.height() * (worker + 1) / workers_count);

            workers.emplace_back([&, startRow, endRow]() {
                for (auto& polygon : prepared_polygons) {
                    for (size_t j = 1; j + 1 < polygon.size(); ++j) {
                        ShadedVertex sv0 = toShadedVertex(polygon[0], fb);
                        ShadedVertex sv1 = toShadedVertex(polygon[j], fb);
                        ShadedVertex sv2 = toShadedVertex(polygon[j + 1], fb);

                        drawTriangle(sv0, sv1, sv2, startRow, endRow, fb, tex, light ? &viewLight : nullptr);
                    }
                }
            });
        }
    }
}
