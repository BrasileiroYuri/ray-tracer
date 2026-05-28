#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"
#include "triangle_mesh.hpp"
#include <memory>

class Triangle : public Shape {
public:
  int *v_;  //! Vertices.
  int *n_;  //! Normais.
  int *uv_; //! Coodernadas u e v.

  std::shared_ptr<TriangleMesh> tmesh_; //! Owner de uma superfície única.

  bool backface_; //! Define se renderiza back.

public:
  Triangle(std::shared_ptr<TriangleMesh> mesh, int id, bool bfc = true)
      : tmesh_{mesh}, backface_{bfc} {

    std::cout << "VRTS:\n";
    v_ = &mesh->vrts_idx_[id * 3];
    for (int i = 0; i < 3; i++)
      std::cout << v_[i] << "\n";

    std::cout << "NORMALS:\n";
    n_ = &mesh->normals_idx_[id * 3];
    for (int i = 0; i < 3; i++)
      std::cout << n_[i] << "\n";

    std::cout << "UV:\n";
    uv_ = &mesh->uv_idxs_[id * 3];
    for (int i = 0; i < 3; i++)
      std::cout << uv_[i] << "\n";
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    //@ TODO
    return true;
  }
};

#endif //! TRIANGLE_HPP
