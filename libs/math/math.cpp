#include "math.hpp"
#include <cmath>
#include <cstddef>
#include <iterator>

// Implementação do Produto Escalar (Dot Product)
float dot(const vec3 &v1, const vec3 &v2) {
  return (v1.i_ * v2.i_) + (v1.j_ * v2.j_) + (v1.k_ * v2.k_);
}

float size(const vec3 &v) { return std::sqrt(dot(v, v)); }

float size(const vec2 &v) { return std::sqrt((v.i_ * v.i_) + (v.j_ * v.j_)); }

vec2 normalize(vec2 v) {
  float s = size(v);
  if (s == 0)
    return v;
  return {v.i_ / s, v.j_ / s};
}

vec3 normalize(vec3 v) {
  float s = size(v);
  if (s == 0)
    return v;
  return {v.i_ / s, v.j_ / s, v.k_ / s};
}

vec3 cross(const vec3 &v1, const vec3 &v2) {
  return {v1.j_ * v2.k_ - v1.k_ * v2.j_, v1.k_ * v2.i_ - v1.i_ * v2.k_,
          v1.i_ * v2.j_ - v1.j_ * v2.i_};
}

inline Matrix4 Matrix4::operator*(const Matrix4 &o) {
  Matrix4 m;
  for (std::size_t l = 0; l < 16; l += 4)
    for (std::size_t c = 0; c < 4; c++)
      m[c + l] =
          values[l + 0] * o.values[c + 0] + values[l + 1] * o.values[c + 4] +
          values[l + 2] * o.values[c + 8] + values[l + 3] * o.values[c + 12];
  return m;
}

inline Matrix4 Matrix4::transpose() const {
  Matrix4 m;

  for (std::size_t i = 0; i < 4; i++) {
    auto base = i * 4;
    m[i + 0] = values[base + 0];
    m[i + 4] = values[base + 1];
    m[i + 8] = values[base + 2];
    m[i + 12] = values[base + 3];
  }
  return m;
}
