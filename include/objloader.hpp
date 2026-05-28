#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "math.hpp"
#include "tiny_obj_loader.h"
#include "triangle_mesh.hpp"
#include <memory>
#include <string>

class ObjLoader {
public:
  static void load(const std::string &filename,
                   std::shared_ptr<TriangleMesh> &mesh) {

    tinyobj::ObjReader obj;
    obj.ParseFromFile(filename);

    const auto &vert = obj.GetAttrib().vertices;
    const auto &norms = obj.GetAttrib().normals;
    const auto &uvs = obj.GetAttrib().texcoords;

    /// Para o TinyObj, as coordenadas estão em floats.
    /// Vertices
    for (auto i = 0; i < vert.size(); i += 3)
      mesh->vertices_.push_back({vec[i], vec[i + 1], vec[i + 2]});

    /// Normais
    for (auto i = 0; i < vec.size(); i += 3)
      mesh->normais.push_back({vec[i], vec[i + 1], vec[i + 2]});
    /// UVs
  }
};

#endif // !OBJLOADER_HPP
