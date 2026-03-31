#ifndef MATH_HPP
#define MATH_HPP

#include <array>
typedef struct vec2 {
  float i_, j_;
  vec2(float i, float j) : i_{i}, j_{j} {}
  inline vec2 operator+(vec2 v) { return {i_ + v.i_, j_ + v.j_}; }
  inline vec2 operator*(float a) { return {i_ * a, j_ * a}; }
  inline void operator*=(float a) {
    i_ *= a;
    j_ *= a;
  }
  inline vec2 operator-(vec2 v) { return {i_ - v.i_, j_ - v.j_}; }
} point2, vec2;

typedef struct vec3 {
  float i_, j_, k_;
  vec3() {}
  vec3(float i, float j, float k) : i_{i}, j_{j}, k_{k} {}
  vec3(const std::array<float, 3> &arr) : i_{arr[0]}, j_{arr[1]}, k_{arr[2]} {}
  inline vec3 operator*(float a) { return {i_ * a, j_ * a, k_ * a}; }
  inline void operator*=(float a) {
    i_ += a;
    j_ *= a;
    k_ *= a;
  }
  inline vec3 operator+(vec3 v) { return {i_ + v.i_, j_ + v.j_, k_ + v.k_}; }
  inline vec3 operator-(vec3 v) { return {i_ - v.i_, j_ - v.j_, k_ - v.k_}; }

} point3, vec3;

vec2 normalize(vec2);
vec3 normalize(vec3);
float size(const vec2 &);
float size(const vec3 &);
vec3 cross(const vec3 &v1, const vec3 &v2);

#endif // !MATH_HPP
