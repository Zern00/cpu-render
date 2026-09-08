#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "glm-like-lib/vectors.hpp"

namespace egn {
    struct Material {
        std::string name;
        gll::Vec3 ambient{0.4f, 0.4f, 0.4f};
        gll::Vec3 diffuse{0.7f, 0.7f, 0.7f};
        gll::Vec3 specular{0.3f, 0.3f, 0.3f};
        gll::Gfloat shininess{32.0f};
    };

    struct Vertex {
        gll::Vec3 pos;
        gll::Vec3 normal;
        gll::Vec2 uv;
        uint32_t color;
    };
    
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        std::vector<Material> materials;
        std::vector<int> triangle_materials;
    };
    
}
