#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
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
    
    std::vector<std::string> SplitByTokens(const std::string& str) {
        std::stringstream ss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    OBJfile ParceObj(const std::filesystem::path& path) {
        OBJfile data;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Can`t open obj file by path " << path.c_str() << std::endl;
            return data;
        }

        std::string cur_line;
        while (std::getline(file, cur_line)) {
            auto tokens = SplitByTokens(cur_line);
             if (tokens.size() == 0) {
                continue;
            }
            std::cerr << cur_line.size() << " ";
            for (auto q : tokens) {
                std::cerr << q << " ";
            }
            std::cerr << std::endl;
            if (tokens[0] == "v") {
                if (tokens.size() >= 4) {
                    gll::Vec3 vec;
                    vec.x = std::stof(tokens[1]);
                    vec.y = std::stof(tokens[2]);
                    vec.z = std::stof(tokens[3]);
                    data.pos_coord.push_back(vec);
                }
            } else if (tokens[0] == "vt") {
                if (tokens.size() >= 3) {
                    gll::Vec2 vec;
                    vec.x = std::stof(tokens[1]);
                    vec.y = std::stof(tokens[2]);
                    data.uv_coord.push_back(vec);
                }
            } else if (tokens[0] == "vn") {
                if (tokens.size() >= 4) {
                    gll::Vec3 vec;
                    vec.x = std::stof(tokens[1]);
                    vec.y = std::stof(tokens[2]);
                    vec.z = std::stof(tokens[3]);
                    data.normal_coord.push_back(vec);
                }
            } else if (tokens[0] == "f") {
                Face f;
                for (size_t i = 1; i < tokens.size(); ++i) {
                    FaceVertex fv{-1, -1, -1};
                    std::string cur_s = tokens[i];
                    
                    for (auto& c : cur_s) {
                        if (c == '/') {
                            c = ' ';
                        }
                    }

                    std::stringstream ss(cur_s);
                    int ind;
                    if (ss >> ind) fv.pos_vertex = ind - 1;
                    if (ss >> ind) fv.uv_vertex = ind - 1;
                    if (ss >> ind) fv.normal_vertex = ind - 1;
                    f.push_back(fv);
                }

                data.faces.push_back(f);
            } else if (tokens[0] == "#") {
                continue;
            } else if (tokens[0] == "mtllib") {
                std::cerr << "mtllib dont support" << std::endl;
            } else {
                std::cerr << "invalid token " << tokens[0] << std::endl;
            }
        }

        file.close();
        return data;
    }

    egn::Mesh ConvertOBJfileToMesh(const OBJfile& obj_file) {
        egn::Mesh mesh;
        size_t count_vert = 0;
        mesh.vertices.reserve(obj_file.faces.size() * 3);
        mesh.indices.reserve(obj_file.faces.size() * 3);

        for (size_t i = 0; i < obj_file.faces.size(); ++i) {
            Face cur_face = obj_file.faces[i];
            for (auto& vertex : cur_face) {
                auto pos = obj_file.pos_coord[vertex.pos_vertex];
                auto normal = obj_file.normal_coord[vertex.normal_vertex];
                auto uv = obj_file.uv_coord[vertex.uv_vertex];
                mesh.vertices.push_back({pos, normal, uv, 
                                           egn::Framebuffer::packColor(255, 255, 255)});
                ++count_vert;
            }
        }

        mesh.indices.resize(count_vert);
        for (size_t i = 0; i < count_vert; ++i) 
            mesh.indices[i] = i;
        return mesh;
    }
}