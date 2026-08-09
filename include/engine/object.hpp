#pragma once
#include <vector>
#include <filesystem>
#include "glm-like-lib/vectors.hpp"
#include "engine/mesh.hpp"
#include "engine/framebuffer.hpp"

namespace obj {
    struct FaceVertex {
        int pos_vertex;
        int uv_vertex;
        int normal_vertex;
    };
    using Face = std::vector<FaceVertex>;

    struct OBJfile {
        std::vector<gll::Vec3> pos_coord;
        std::vector<gll::Vec2> uv_coord;
        std::vector<gll::Vec3> normal_coord;
        std::vector<Face> faces;
    };
    
    std::vector<std::string> SplitByTokens(const std::string& str);

    OBJfile ParceObj(const std::filesystem::path& path);

    egn::Mesh ConvertOBJfileToMesh(const OBJfile& obj_file);
}
