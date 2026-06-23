#ifndef MATH_HPP
#define MATH_HPP

#include <array>
#include <iterator>
#include <sstream>
#include <string>

typedef struct vec2 {
  float i_, j_;

  vec2(const std::array<float, 3> &arr) : i_{arr[0]}, j_{arr[1]} {}

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

inline std::istream &operator>>(std::istream &is, vec3 &v) {
  if (!(is >> v.i_ >> v.j_ >> v.k_))
    is.setstate(std::ios::failbit);
  return is;
}
inline std::istream &operator>>(std::istream &is, vec2 &v) {
  if (!(is >> v.i_ >> v.j_))
    is.setstate(std::ios::failbit);
  return is;
}

struct Matrix4 {
  double values[16]{}; //! Iniciando todos os valores com 0.

  inline Matrix4 operator*(const Matrix4 &other);
  inline Matrix4 transpose() const;
  inline double &operator[](std::size_t idx) { return values[idx]; };
};

class Transform {
public:
  Transform(const Matrix4 &m, const Matrix4 &mInv) : m{m}, mInv{mInv} {}
  Transform translate(const vec3 &v) const {
    Matrix4 mn{
        1, 0, 0, v.i_, //
        0, 1, 0, v.j_, //
        0, 0, 1, v.k_, //
        0, 0, 0, 1,    //
    };

    Matrix4 mi{
        1, 0, 0, -v.i_, //
        0, 1, 0, -v.j_, //
        0, 0, 1, -v.k_, //
        0, 0, 0, 1,     //
    };

    return Transform(mn, mi);
  }

  Transform scale(const vec3 &v) const {
    Matrix4 mn{
        v.i_, 0,    0,    0, //
        0,    v.j_, 0,    0, //
        0,    0,    v.k_, 0, //
        0,    0,    0,    1, //
    };

    Matrix4 mi{
        1 / v.i_, 0,        0,        0, //
        0,        1 / v.j_, 0,        0, //
        0,        0,        1 / v.k_, 0, //
        0,        0,        0,        1, //
    };

    return Transform(mn, mi);
  }

private:
  Matrix4 m, mInv;
};
#endif // !MATH_HPP
