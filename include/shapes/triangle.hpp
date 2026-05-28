#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"
#include <memory>

class Triangle : public Shape {
private:
  int *v_;  //! Vertices.
  int *n_;  //! Normais.
  int *uv_; //! Coodernadas u e v.

  std::shared_ptr<TriangleMesh> tmesh_; //! Owner de uma superfície única.

  bool backface_; //! Define se renderiza back.

public:
  Triangle(std::shared_ptr<TriangleMesh> mesh, int id, bool bfc = true)
      : tmesh_{mesh}, backface_{bfc} {

    v_ = &mesh->vrts_idx_[id * 3];
    n_ = &mesh->normals_idx_[id * 3];
    uv_ = &mesh->uv_idxs_[id * 3];
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    //@ TODO
    return true;
  }
};

#endif //! TRIANGLE_HPP
