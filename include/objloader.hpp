#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "tiny_obj_loader.h"
#include "triangle.hpp"
#include "triangle_mesh.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ld {

inline std::vector<Triangle> load(const std::string &filename,
                                  std::shared_ptr<TriangleMesh> &tmesh) {

  tinyobj::ObjReader obj;
  tinyobj::ObjReaderConfig objConfig;
  objConfig.triangulate = true; /// Superficie triangular

  if (!obj.ParseFromFile(filename, objConfig)) {
    if (!obj.Error().empty())
      std::cerr << ">>> TinyObjLoader: " << obj.Error() << "\n";
    exit(0);
  }

  if (!obj.Warning().empty()) {
    std::cout << ">>> TinyObjReader:\n" << obj.Warning();
  }

  /// Para o TinyObj, as coordenadas são trios de floats.
  /// Por eficiência, eu uso move (O(1)) e converto para Point de forma "Lazy",
  /// ao invés de já converter (O(N)).
  tmesh->vertices_ = std::move(obj.GetAttrib().vertices);
  tmesh->normals_ = std::move(obj.GetAttrib().normals);
  tmesh->uvcoords_ = std::move(obj.GetAttrib().texcoords);

  /// Todos os shapes (no .obg, g ou o).
  const auto &shapes = obj.GetShapes();

  std::vector<Triangle> tr;

  int t_idx = 0;

  /// Para cade Shape (ou malha nomeada).
  for (const auto &shape : shapes) {
    auto mesh = shape.mesh; /// A malha propriamente dita

    auto idx_off = 0;
    /// Percorremos um conjunto de faces, ou polígonos 2d.
    /// Supondo que uma malha tenha 4 triangulos, teriamos [3,3,3,3]
    for (size_t i = 0; i < mesh.num_face_vertices.size(); i++) {

      auto f =
          (int)mesh.num_face_vertices[i]; /// Qtd de vértices do poligono 2d.

      /// Percorremos f vezes pois cada vez nos gera um index_t, que dá
      /// acesso ao indice de vertice, normal e textura (ou uv).
      for (auto k = 0; k < f; k++) {
        tinyobj::index_t index =
            mesh.indices[idx_off++]; /// Note que em faces consecutivas de uma
                                     /// malha (e dentro da própria face) os
                                     /// index_t são consecutivos.

        /// Adiciono na mesma ordem do TinyObjLoader, apenas salvo o Triangle
        /// como o primeiro. No caso, o triangle_idx.
        tmesh->vrts_idx_.push_back(index.vertex_index);
        tmesh->normals_idx_.push_back(index.normal_index);
        tmesh->uv_idxs_.push_back(index.texcoord_index);
      }

      tr.push_back({tmesh, t_idx++});
    }
  }
  return tr;
}
} // namespace ld

#endif // !OBJLOADER_HPP
