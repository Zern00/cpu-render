#include "object.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace obj {
    std::vector<std::string> SplitByTokens(const std::string& str) {
        std::stringstream ss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    egn::Material* FindMaterial(std::vector<egn::Material>& materials, const std::string& name) {
        auto material = std::find_if(materials.begin(), materials.end(), [&name](const egn::Material& value) {
            return value.name == name;
        });
        return material == materials.end() ? nullptr : &*material;
    }

    void ParseMaterialLibrary(const std::filesystem::path& path, std::vector<egn::Material>& materials) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Can`t open material file by path " << path.c_str() << std::endl;
            return;
        }

        egn::Material* current_material = nullptr;
        std::string cur_line;
        while (std::getline(file, cur_line)) {
            auto tokens = SplitByTokens(cur_line);
            if (tokens.empty() || tokens[0][0] == '#') {
                continue;
            }

            if (tokens[0] == "newmtl" && tokens.size() >= 2) {
                current_material = FindMaterial(materials, tokens[1]);
                if (!current_material) {
                    materials.push_back(egn::Material{});
                    current_material = &materials.back();
                    current_material->name = tokens[1];
                }
            } else if (current_material && tokens[0] == "Ka" && tokens.size() >= 4) {
                current_material->ambient = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
            } else if (current_material && tokens[0] == "Kd" && tokens.size() >= 4) {
                current_material->diffuse = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
            } else if (current_material && tokens[0] == "Ks" && tokens.size() >= 4) {
                current_material->specular = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
            } else if (current_material && tokens[0] == "Ns" && tokens.size() >= 2) {
                current_material->shininess = std::stof(tokens[1]);
            }
        }
    }

    OBJfile ParceObj(const std::filesystem::path& path) {
        OBJfile data;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Can`t open obj file by path " << path.c_str() << std::endl;
            return data;
        }

        std::string cur_line;
        std::string current_material;
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
                data.face_materials.push_back(current_material);
            } else if (tokens[0] == "#") {
                continue;
            } else if (tokens[0] == "mtllib") {
                for (size_t i = 1; i < tokens.size(); ++i) {
                    ParseMaterialLibrary(path.parent_path() / tokens[i], data.materials);
                }
            } else if (tokens[0] == "usemtl" && tokens.size() >= 2) {
                current_material = tokens[1];
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
        mesh.materials = obj_file.materials;
        mesh.triangle_materials.reserve(obj_file.faces.size());

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

            int material_index = -1;
            if (i < obj_file.face_materials.size()) {
                auto material = std::find_if(mesh.materials.begin(), mesh.materials.end(), [&obj_file, i](const egn::Material& value) {
                    return value.name == obj_file.face_materials[i];
                });
                if (material != mesh.materials.end()) {
                    material_index = static_cast<int>(std::distance(mesh.materials.begin(), material));
                }
            }
            mesh.triangle_materials.push_back(material_index);
        }

        mesh.indices.resize(count_vert);
        for (size_t i = 0; i < count_vert; ++i) 
            mesh.indices[i] = i;
        return mesh;
    }
}
