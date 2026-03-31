#include "math.hpp"
#include <math.h>

float size(const vec3 &v) {
  return std::sqrt((v.i_ * v.i_) + (v.j_ * v.j_) + (v.k_ * v.k_));
}

float size(const vec2 &v) { return std::sqrt((v.i_ * v.i_) + (v.j_ * v.j_)); }

vec2 normalize(vec2 v) {
  float i = size(v);
  return {v.i_ / i, v.j_ / i};
}

vec3 normalize(vec3 v) {
  float i = size(v);
  return {v.i_ / i, v.j_ / i, v.k_ / i};
}

vec3 cross(const vec3 &v1, const vec3 &v2) {
  float x, y, z;
  x = v1.j_ * v2.k_ - v1.k_ * v2.j_;
  y = v1.k_ * v2.i_ - v1.i_ * v2.k_;
  z = v1.i_ * v2.j_ - v1.j_ * v2.i_;

  return {x, y, z};
}
