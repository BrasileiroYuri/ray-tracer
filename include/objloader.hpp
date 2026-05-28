#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "math.hpp"
#include "tiny_obj_loader.h"
#include "triangle.hpp"
#include "triangle_mesh.hpp"
#include <memory>
#include <string>

namespace ld {

void load(const std::string &filename, std::shared_ptr<TriangleMesh> &tmesh) {

  tinyobj::ObjReader obj;
  tinyobj::ObjReaderConfig objConfig;
  objConfig.triangulate = true; /// Superficie triangular

  if (!obj.ParseFromFile(filename, objConfig)) {
    if (!obj.Error().empty())
      std::cerr << "TinyObjLoader: " << obj.Error() << "\n";
    exit(0);
  }

  if (!obj.Warning().empty()) {
    std::cout << "TinyObjReader: " << obj.Warning();
  }

  /// Para o TinyObj, as coordenadas são trios de floats.
  /// Por eficiência, eu uso move e converto para Point de forma "Lazy".
  tmesh->vertices_ = std::move(obj.GetAttrib().vertices);
  tmesh->normals_ = std::move(obj.GetAttrib().normals);
  tmesh->uvcoords_ = std::move(obj.GetAttrib().texcoords);

  const auto &shapes = obj.GetShapes();

  std::vector<Triangle> tr;

  int triangle_idx = 0;
  /// Usado para criar objetos.
  /// Para cade Shape, como Triangle, Cubes, etc.
  for (const auto &shape : shapes) {
    auto mesh = shape.mesh;

    auto idx_off = 0;
    for (auto i = 0; i < mesh.num_face_vertices.size(); i++) {
      auto faces = (int)mesh.num_face_vertices[i];
      std::cout << "FACES: " << faces
                << "\n------------------------------------\n";

      for (auto k = 0; k < faces; k++) {
        tinyobj::index_t index = mesh.indices[idx_off++];

        /// Adiciono na mesma ordem do TinyObjLoader, apenas salvo o Triangle
        /// como o primeiro.
        std::cout << "vertex_index: " << index.vertex_index << "\n";
        tmesh->vrts_idx_.push_back(index.vertex_index);

        std::cout << "normal_index: " << index.normal_index << "\n";
        tmesh->normals_idx_.push_back(index.normal_index);

        std::cout << "tex_index: " << index.texcoord_index << "\n";
        tmesh->uv_idxs_.push_back(index.texcoord_index);
      }

      tr.push_back((Triangle){tmesh, triangle_idx++});
    }
  }

  for (const auto &elem : tr) {
    std::cout << "ELEM V_:\n";
    for (int i = 0; i < 3; i++)
      std::cout << elem.v_[i] << "\n";

    std::cout << "ELEM N_:\n";
    for (int i = 0; i < 3; i++)
      std::cout << elem.n_[i] << "\n";

    std::cout << "ELEM UV_:\n";
    for (int i = 0; i < 3; i++)
      std::cout << elem.uv_[i] << "\n";
  }
}
} // namespace ld

#endif // !OBJLOADER_HPP
