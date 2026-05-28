#ifndef TRIANGLE_MESH_HPP
#define TRIANGLE_MESH_HPP

#include "math.hpp"

struct TriangleMesh {
  std::size_t ntriangles; //! Número de triangles da superficie.

  /*
   * Cada lista abaixo tem 3 * ntriangles.
   */
  std::vector<int> vrts_idx_;    //!
  std::vector<int> normals_idx_; //!
  std::vector<int> uv_idxs_;     //!

  std::vector<float> vertices_; //! Conjunto de todos os vértices.
  std::vector<float> normals_;  //! Conjunto de todos os vetores normais.
  std::vector<float> uvcoords_; //! Coodernadas das texturas.
};

#endif //! TRIANGLE_MESH_HPP
