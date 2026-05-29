#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "math.hpp"
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include <memory>
#include <vector>

class Triangle : public Shape {
private:
  /*
   * Indice usado para recuperar os 3 arrays (vertices, normais e uvs)
   * com mesh->array_correspondente[id].
   */
  int id_;

  std::shared_ptr<TriangleMesh> tmesh_; //! Owner de uma superfície única.

  bool backface_; //! Define se renderiza back.

  point3 calculate_p3(const std::vector<float> &vec, int idx) const {
    auto t = &vec[idx * 3];
    return {t[0], t[1], t[2]};
  }

  point2 calculate_p2(const std::vector<float> &vec, int idx) const {
    auto t = &vec[idx * 2];
    return {t[0], t[1]};
  }

public:
  Triangle(std::shared_ptr<TriangleMesh> mesh, int id, bool bfc = true)
      : id_{id}, tmesh_{mesh}, backface_{bfc} {}

  bool intersect(const Ray &r, Surfel &s) const override {
    auto *v_ = &tmesh_->vrts_idx_[id_ * 3];
    auto *n_ = &tmesh_->normals_idx_[id_ * 3];
    auto *uv_ = &tmesh_->uv_idxs_[id_ * 3];

    const auto v1 = calculate_p3(tmesh_->vertices_, v_[0]);
    const auto v2 = calculate_p3(tmesh_->vertices_, v_[1]);
    const auto v3 = calculate_p3(tmesh_->vertices_, v_[2]);

    const auto n1 = calculate_p3(tmesh_->normals_, n_[0]);
    const auto n2 = calculate_p3(tmesh_->normals_, n_[1]);
    const auto n3 = calculate_p3(tmesh_->normals_, n_[2]);

    const auto uv1 = calculate_p2(tmesh_->uvcoords_, uv_[0]);
    const auto uv2 = calculate_p2(tmesh_->uvcoords_, uv_[1]);
    const auto uv3 = calculate_p2(tmesh_->uvcoords_, uv_[2]);

    //@ TODO
    return true;
  }
};

#endif //! TRIANGLE_HPP
