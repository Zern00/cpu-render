#pragma once
#include <cstdint>
#include <vector>
#include "glm-like-lib/vectors.hpp"

namespace egn {
    struct Vertex {
        gll::Vec3 pos;
        gll::Vec3 normal;
        gll::Vec2 uv;
        uint32_t color;
    };
    
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<int> indices;
    };
    
}