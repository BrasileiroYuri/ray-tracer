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
    // 1. Mapeamento de índices e extração de dados brutos
    auto v_ = &tmesh_->vrts_idx_[id_ * 3];
    auto n_ = &tmesh_->normals_idx_[id_ * 3];

    const auto &v1 = calculate_p3(tmesh_->vertices_, v_[0]);
    const auto &v2 = calculate_p3(tmesh_->vertices_, v_[1]);
    const auto &v3 = calculate_p3(tmesh_->vertices_, v_[2]);

    const auto &n1 = calculate_p3(tmesh_->normals_, n_[0]);
    const auto &n2 = calculate_p3(tmesh_->normals_, n_[1]);
    const auto &n3 = calculate_p3(tmesh_->normals_, n_[2]);

    // 2. Arestas do triângulo
    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    // 3. Início do algoritmo Möller-Trumbore
    auto p = cross(r.direction_, e2);
    float det = dot(p, e1); // Equivalente ao produto triplo escalar

    // Aplicação do Backface Culling (Sistema Left-Handed: det < 0 é frontal)
    if (backface_ && det >= 0.0f)
      return false;

    // Verifica se o raio é paralelo ao triângulo (evita divisão por zero)
    if (det == 0.0f)
      return false;

    float fct = 1.0f / det;

    auto tvec = r.origin_ - v1;
    auto q = cross(tvec, e1);

    // 4. Cálculo das coordenadas baricêntricas (u, v) e distância t
    float u = dot(p, tvec) * fct;
    float v = dot(q, r.direction_) * fct;
    s.t_hit = dot(q, e2) * fct;

    // 5. Validação rigorosa dos limites de intersecção
    // w = 1.0 - u - v, portanto u + v > 1.0 significa w < 0.0 (fora do
    // triângulo)
    // CORRETO: tolerância mínima nas bordas
    const float eps = 1e-5f;
    if (u < -eps || v < -eps || u + v > 1.0f + eps || s.t_hit < r.min_t_ ||
        s.t_hit > r.max_t_)
      return false;
    // Interpolação baricêntrica das normais dos vértices
    s.n = (1.0f - u - v) * n1 + u * n2 + v * n3;

    // Normalização crucial para não quebrar a equação especular de Blinn-Phong
    s.n = normalize(s.n);

    // Inversão da normal ao atingir a face traseira (quando o culling está
    // desligado)
    if (!backface_ && det > 0.0f) {
      s.n = -1 * s.n;
    }
    // O "Pulo do Gato": Adiciona um epsilon empurrando o ponto para fora
    s.p = r(s.t_hit);
    return true;
  }
};

#endif //! TRIANGLE_HPP
