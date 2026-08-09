#include "rasterizer.hpp"
#include <algorithm>
#include <cmath>

namespace egn {
    void drawTriangle(const egn::ShadedVertex& v0, const egn::ShadedVertex& v1, const egn::ShadedVertex& v2, 
                      Framebuffer& fb, const Texture* tex, const Light* light) {
        //AABB
        float minX = std::max(0.0f, std::floor(std::min(v0.screenPos.x, std::min(v1.screenPos.x, v2.screenPos.x))));
        float maxX = std::min(static_cast<float>(fb.width() - 1), std::ceil(std::max(v0.screenPos.x, std::max(v1.screenPos.x, v2.screenPos.x))));
        float minY = std::max(0.0f, std::floor(std::min(v0.screenPos.y, std::min(v1.screenPos.y, v2.screenPos.y))));
        float maxY = std::min(static_cast<float>(fb.height() - 1), std::ceil(std::max(v0.screenPos.y, std::max(v1.screenPos.y, v2.screenPos.y))));

        gll::Gfloat area = (v1.screenPos.x - v0.screenPos.x) * (v2.screenPos.y - v0.screenPos.y) - (v1.screenPos.y - v0.screenPos.y) * (v2.screenPos.x - v0.screenPos.x);
        if (area >= 0.0f) return;

        gll::Gfloat invArea = 1 / area;

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                gll::Gfloat px = x + 0.5f;
                gll::Gfloat py = y + 0.5f;


                gll::Gfloat w0 = (v2.screenPos.x - v1.screenPos.x) * (py - v1.screenPos.y) - (v2.screenPos.y - v1.screenPos.y) * (px - v1.screenPos.x);
                gll::Gfloat w1 = (v0.screenPos.x - v2.screenPos.x) * (py - v2.screenPos.y) - (v0.screenPos.y - v2.screenPos.y) * (px - v2.screenPos.x);
                gll::Gfloat w2 = (v1.screenPos.x - v0.screenPos.x) * (py - v0.screenPos.y) - (v1.screenPos.y - v0.screenPos.y) * (px - v0.screenPos.x);


                bool inside = (area > 0) ? (w0 >= 0 && w1 >= 0 && w2 >= 0) : (w0 <= 0 && w1 <= 0 && w2 <= 0);

                if (inside) {
                    gll::Gfloat alpha = w0 * invArea;
                    gll::Gfloat beta = w1 * invArea;
                    gll::Gfloat gamma = w2 * invArea;

                    gll::Gfloat z = alpha * v0.screenPos.z + beta * v1.screenPos.z + gamma * v2.screenPos.z;

                    if (z < fb.getDepth(x, y)) {
                        gll::Gfloat invW = alpha * v0.invW + beta * v1.invW + gamma * v2.invW;

                        gll::Gfloat u = (alpha * v0.uv.x + beta * v1.uv.x + gamma * v2.uv.x) / invW;
                        gll::Gfloat v = (alpha * v0.uv.y + beta * v1.uv.y + gamma * v2.uv.y) / invW;

                        uint32_t texColor = 0xFFFFFFFF;
                        if (tex) {
                            texColor = tex->sample(u, v);
                        }

                        uint8_t b0 = (v0.color >> 16) & 0xFF, g0 = (v0.color >> 8) & 0xFF, r0 = v0.color & 0xFF;
                        uint8_t b1 = (v1.color >> 16) & 0xFF, g1 = (v1.color >> 8) & 0xFF, r1 = v1.color & 0xFF;
                        uint8_t b2 = (v2.color >> 16) & 0xFF, g2 = (v2.color >> 8) & 0xFF, r2 = v2.color & 0xFF;

                        uint8_t r = static_cast<uint8_t>((alpha * r0 + beta * r1 + gamma * r2) / invW);
                        uint8_t g = static_cast<uint8_t>((alpha * g0 + beta * g1 + gamma * g2) / invW);
                        uint8_t b = static_cast<uint8_t>((alpha * b0 + beta * b1 + gamma * b2) / invW);

                        uint8_t bTex = (texColor >> 16) & 0xFF;
                        uint8_t gTex = (texColor >> 8) & 0xFF;
                        uint8_t rTex = texColor & 0xFF;

                        r = (r * rTex) / 255;
                        g = (g * gTex) / 255;
                        b = (b * bTex) / 255;

                        if (light) {
                            gll::Vec3 viewPos;
                            viewPos.x = (alpha * v0.viewPos.x + beta * v1.viewPos.x + gamma * v2.viewPos.x) / invW;
                            viewPos.y = (alpha * v0.viewPos.y + beta * v1.viewPos.y + gamma * v2.viewPos.y) / invW;
                            viewPos.z = (alpha * v0.viewPos.z + beta * v1.viewPos.z + gamma * v2.viewPos.z) / invW;

                            gll::Vec3 normal;
                            normal.x = (alpha * v0.normal.x + beta * v1.normal.x + gamma * v2.normal.x) / invW;
                            normal.y = (alpha * v0.normal.y + beta * v1.normal.y + gamma * v2.normal.y) / invW;
                            normal.z = (alpha * v0.normal.z + beta * v1.normal.z + gamma * v2.normal.z) / invW;
                            normal = normal.normalized();

                            gll::Vec3 viewDir = (gll::Vec3{0,0,0} - viewPos).normalized();

                            gll::Vec3 lightDir = light->direction.normalized();

                            // Diffuse
                            gll::Gfloat diff = std::max(dot_product(normal, lightDir), gll::Gfloat(0.0f));

                            // Specular (Blinn-Phong)
                            gll::Vec3 halfway = (lightDir + viewDir).normalized();
                            gll::Gfloat spec = std::pow(std::max(dot_product(normal, halfway), gll::Gfloat(0.0f)), light->shininess);

                            gll::Gfloat lighting = light->ambient + light->diffuse * diff + light->specular * spec;
                            lighting = std::min(lighting, gll::Gfloat(1.0f));

                            r *= lighting;
                            g *= lighting;
                            b *= lighting;
                        }
                        
                        uint32_t finalColor = Framebuffer::packColor(r, g, b);
                        fb.setDepth(x, y, z);
                        fb.setPixel(x, y, finalColor);
                    }
                }
            }
        }
    }
}
