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
    // 1. Extração de vértices
    auto v_ = &tmesh_->vrts_idx_[id_ * 3];
    const auto v1 = calculate_p3(tmesh_->vertices_, v_[0]);
    const auto v2 = calculate_p3(tmesh_->vertices_, v_[1]);
    const auto v3 = calculate_p3(tmesh_->vertices_, v_[2]);

    // 2. Arestas do triângulo
    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    // 3. Möller-Trumbore
    auto p = cross(r.direction_, e2);
    float det = dot(p, e1);

    // Raio paralelo ao triângulo
    if (std::fabs(det) < 1e-8f)
      return false;

    float fct = 1.0f / det;
    auto tvec = r.origin_ - v1;
    auto q = cross(tvec, e1);

    // 4. Coordenadas baricêntricas e t
    float u = dot(p, tvec) * fct;
    float v = dot(q, r.direction_) * fct;
    s.t_hit = dot(q, e2) * fct;

    // 5. Validação de interseção
    const float eps = 1e-5f;
    if (u < -eps || v < -eps || u + v > 1.0f + eps || s.t_hit < r.min_t_ ||
        s.t_hit > r.max_t_)
      return false;

    // ========================================================================
    // 6. SOLUÇÃO DEFINITIVA: Winding Dinâmico e Culling Baseado em Normais
    // ========================================================================

    // Calculamos a normal geométrica bruta
    auto geom_n = normalize(cross(e1, e2));
    bool has_normals =
        !tmesh_->normals_.empty() && !tmesh_->normals_idx_.empty();

    if (has_normals) {
      auto n_ = &tmesh_->normals_idx_[id_ * 3];
      if (n_[0] >= 0) {
        vec3 vertex_normal = calculate_p3(tmesh_->normals_, n_[0]);
        // Se a normal geométrica calculada for oposta à normal fornecida pelo
        // modelo (que sabemos apontar sempre para o exterior), o winding do
        // triângulo é CW. Invertemos geom_n para que ela aponte confiavelmente
        // para fora.
        if (dot(geom_n, vertex_normal) < 0.0f) {
          geom_n = geom_n * -1.0f;
        }
      }
    } else {
      // Fallback: Na regra da mão esquerda, malhas CCW comuns têm cross(e1,e2)
      // apontando para dentro do volume. Invertemos para apontar para fora.
      geom_n = geom_n * -1.0f;
    }

    // O triângulo é de fato uma "Face Frontal" se a direção do raio
    // for oposta à normal exterior (estão de frente um para o outro).
    bool is_front_face = dot(r.direction_, geom_n) < 0.0f;

    // Culling Seguro: Descartes a face se for traseira e o culling estiver
    // ativo
    if (backface_ && !is_front_face)
      return false;

    // 7. Configurando as normais no Surfel
    // A normal geométrica do Surfel precisa de apontar contra o raio (em
    // direção à câmera) para que a proteção de Shadow Bias jogue o offset para
    // o ar.
    s.geom_n = is_front_face ? geom_n : (geom_n * -1.0f);

    // Interpolação baricêntrica suave da normal de Shading
    if (has_normals) {
      auto n_ = &tmesh_->normals_idx_[id_ * 3];
      if (n_[0] >= 0 && n_[1] >= 0 && n_[2] >= 0) {
        const auto n1 = calculate_p3(tmesh_->normals_, n_[0]);
        const auto n2 = calculate_p3(tmesh_->normals_, n_[1]);
        const auto n3 = calculate_p3(tmesh_->normals_, n_[2]);
        s.n = normalize((1.0f - u - v) * n1 + u * n2 + v * n3);
      } else {
        s.n = s.geom_n;
      }
    } else {
      s.n = s.geom_n;
    }

    // Se estivermos a bater numa face traseira (Culling desativado),
    // invertemos a normal de shading para ela captar luz do lado de dentro.
    if (!is_front_face) {
      s.n = s.n * -1.0f;
    }

    // ========================================================================

    // 8. UV coordinates — interpoladas se disponíveis, baricêntricas como
    // fallback
    bool has_uvs = !tmesh_->uvcoords_.empty() && !tmesh_->uv_idxs_.empty();
    if (has_uvs) {
      auto uv_ = &tmesh_->uv_idxs_[id_ * 3];
      if (uv_[0] >= 0 && uv_[1] >= 0 && uv_[2] >= 0) {
        const auto uv1 = calculate_p2(tmesh_->uvcoords_, uv_[0]);
        const auto uv2 = calculate_p2(tmesh_->uvcoords_, uv_[1]);
        const auto uv3 = calculate_p2(tmesh_->uvcoords_, uv_[2]);
        float w = 1.0f - u - v;
        s.uv = {w * uv1.i_ + u * uv2.i_ + v * uv3.i_,
                w * uv1.j_ + u * uv2.j_ + v * uv3.j_};
      } else {
        s.uv = {u, v};
      }
    } else {
      s.uv = {u, v}; // fallback: baricêntricas brutas como UV
    }

    s.p = r(s.t_hit);
    return true;
  }
};

#endif //! TRIANGLE_HPP
