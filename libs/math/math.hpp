#ifndef MATH_HPP
#define MATH_HPP

#include <array>
#include <sstream>
#include <string>

typedef struct vec2 {
  float i_, j_;
  vec2(float i = 0, float j = 0) : i_{i}, j_{j} {}
  inline vec2 operator+(const vec2 &v) const { return {i_ + v.i_, j_ + v.j_}; }
  inline vec2 operator-(const vec2 &v) const { return {i_ - v.i_, j_ - v.j_}; }
  inline vec2 operator*(float a) const { return {i_ * a, j_ * a}; }
  inline void operator*=(float a) {
    i_ *= a;
    j_ *= a;
  }

  std::string str() const {
    std::stringstream ss;
    ss << "{" << i_ << ", " << j_ << "}";
    return ss.str();
  }

} point2, vec2;

typedef struct vec3 {
  float i_, j_, k_;
  vec3(float i = 0, float j = 0, float k = 0) : i_{i}, j_{j}, k_{k} {}
  vec3(const std::array<float, 3> &arr) : i_{arr[0]}, j_{arr[1]}, k_{arr[2]} {}

  inline vec3 operator+(const vec3 &v) const {
    return {i_ + v.i_, j_ + v.j_, k_ + v.k_};
  }
  inline vec3 operator-(const vec3 &v) const {
    return {i_ - v.i_, j_ - v.j_, k_ - v.k_};
  }
  inline vec3 operator*(float a) const { return {i_ * a, j_ * a, k_ * a}; }
  inline vec3 operator*(const vec3 &v) const {
    return {i_ * v.i_, j_ * v.j_, k_ * v.k_};
  }
  inline vec3 operator/(const vec3 &v) const {
    return {i_ / v.i_, j_ / v.j_, k_ / v.k_};
  }

  inline void operator*=(float a) {
    i_ *= a;
    j_ *= a;
    k_ *= a;
  }

  std::string str() const {
    std::stringstream ss;
    ss << "{" << i_ << ", " << j_ << ", " << k_ << "}";
    return ss.str();
  }

} point3, vec3;

// Operadores globais para permitir float * vec3 (Necessário para o Ray)
inline vec3 operator*(float a, const vec3 &v) { return v * a; }

// Funções Geométricas
float dot(const vec3 &v1, const vec3 &v2);
vec2 normalize(vec2);
vec3 normalize(vec3);
float size(const vec2 &);
float size(const vec3 &);
vec3 cross(const vec3 &v1, const vec3 &v2);

#endif // !MATH_HPP
