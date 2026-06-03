#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "math.hpp"
#include "primitive.hpp"
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include <iostream>
#include <memory>
#include <vector>

class Triangle : public Shape {
private:
  int id_; //! Id da intância do Triangle nos vetores de vértices, normais e
           //! uvs.

  std::shared_ptr<TriangleMesh> tmesh_; //! Owner de uma superfície única.

  bool backface_; //! Define se renderiza back.

  inline point3 calculate_p3(const std::vector<float> &vec, int idx) const {
    auto t = &vec[idx * 3];
    return {t[0], t[1], t[2]};
  }

  inline point2 calculate_p2(const std::vector<float> &vec, int idx) const {
    auto t = &vec[idx * 2];
    return {t[0], t[1]};
  }

public:
  Triangle(std::shared_ptr<TriangleMesh> &mesh, int id, bool bfc = true)
      : id_{id}, tmesh_{mesh}, backface_{bfc} {}

  bool intersect(const Ray &r, Surfel &s) const override {

    auto v_ = &tmesh_->vrts_idx_[id_ * 3];
    auto n_ = &tmesh_->normals_idx_[id_ * 3];
    // auto uv_ = &tmesh_->uv_idxs_[id_ * 3];

    const auto &v1 = calculate_p3(tmesh_->vertices_, v_[0]);
    const auto &v2 = calculate_p3(tmesh_->vertices_, v_[1]);
    const auto &v3 = calculate_p3(tmesh_->vertices_, v_[2]);

    const auto &n1 = calculate_p3(tmesh_->normals_, n_[0]);
    const auto &n2 = calculate_p3(tmesh_->normals_, n_[1]);
    const auto &n3 = calculate_p3(tmesh_->normals_, n_[2]);

    /*
    const auto &uv1 = calculate_p2(tmesh_->uvcoords_, uv_[0]);
    const auto &uv2 = calculate_p2(tmesh_->uvcoords_, uv_[1]);
    const auto &uv3 = calculate_p2(tmesh_->uvcoords_, uv_[2]);
    */

    /// Achando vetores que compartilham v1 como origem.
    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    /// Aqui seria a interpolação das normais?
    vec3 triangle_normal = cross(e1, e2);

    if (dot(triangle_normal, r.direction_) <= 0)
      return false; /// Paralelo ao plano que contém o triangle.

    auto t = r.origin_ - v1;
    auto p = cross(r.direction_, e2);
    auto q = cross(t, e1);

    auto fct = 1 / dot(p, e1);

    s.t_hit = dot(q, e2) * fct;
    auto u = dot(p, t) * fct;
    auto v = dot(q, r.direction_) * fct;

    if (u < 0 || v < 0 || s.t_hit <= 0)
      return false;

    s.p = r(s.t_hit);
    s.n = (1 - u - v) * n1 + u * n2 + v * n3;
    return true;
  }
};

#endif //! TRIANGLE_HPP
