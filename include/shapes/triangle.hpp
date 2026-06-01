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

    const auto &v1 = calculate_p3(tmesh_->vertices_, v_[0]);
    const auto &v2 = calculate_p3(tmesh_->vertices_, v_[1]);
    const auto &v3 = calculate_p3(tmesh_->vertices_, v_[2]);

    const auto &n1 = calculate_p3(tmesh_->normals_, n_[0]);
    const auto &n2 = calculate_p3(tmesh_->normals_, n_[1]);
    const auto &n3 = calculate_p3(tmesh_->normals_, n_[2]);

    const auto &uv1 = calculate_p2(tmesh_->uvcoords_, uv_[0]);
    const auto &uv2 = calculate_p2(tmesh_->uvcoords_, uv_[1]);
    const auto &uv3 = calculate_p2(tmesh_->uvcoords_, uv_[2]);

    /// Achando vetores que compartilham v1 como origem.
    auto vec1 = v2 - v1;
    auto vec2 = v3 - v1;

    auto cr = cross(r.direction_, vec2);
    auto det = dot(vec1, cr);

    std::cout << "DET: " << det << "\n";
    if (det > -0.0001 && det < 0.0001)
      return false;

    auto inv_det = 1.0 / det;

    auto dist = r.origin_ - v1;

    auto u = dot(dist, cr) * inv_det;
    if (u < 0.0 || u > 1.0)
      return false;

    auto vec4 = cross(dist, vec1);
    auto v = dot(r.direction_, vec4) * inv_det;
    if (v < 0.0 || v > 1.0)
      return false;

    s.t_hit = dot(vec2, vec4) * inv_det;

    return true;
  }
};

#endif //! TRIANGLE_HPP
