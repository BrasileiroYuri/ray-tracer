#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "math.hpp"
#include <array>
#include <sstream>
#include <string>

struct RGBColor {

  float r_ = 0, g_ = 0, b_ = 0, a_ = 1.0f;

  // Construtor direto
  RGBColor(float r, float g, float b, float a = 1.0f)
      : r_{r}, g_{g}, b_{b}, a_{a} {}

  RGBColor() = default;

  // Operadores necessários para as fórmulas: L = Ia*ka + Id*kd + Is*ks
  inline RGBColor operator*(const RGBColor &other) const {
    return {r_ * other.r_, g_ * other.g_, b_ * other.b_, a_ * other.a_};
  }

  inline RGBColor operator*(const vec3 &v) const {
    return {r_ * v.i_, g_ * v.j_, b_ * v.k_};
  }

  inline RGBColor operator*(float s) const {
    return {r_ * s, g_ * s, b_ * s, a_};
  }

  inline RGBColor operator+(const RGBColor &other) const {
    return {r_ + other.r_, g_ + other.g_, b_ + other.b_, a_};
  }

  RGBColor &operator+=(const RGBColor &other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;
    return *this;
  }

  std::string str() const {
    std::stringstream ss;
    // Converte os valores de 0.0-1.0 para inteiros 0-255 para exibição no log
    ss << "{" << static_cast<int>(r_ * 255) << ", "
       << static_cast<int>(g_ * 255) << ", " << static_cast<int>(b_ * 255)
       << "}";
    return ss.str();
  }

  // Método auxiliar para o Integrator obter a cor base em materiais não-Blinn
  RGBColor getColor() const { return *this; }
};

inline std::istream &operator>>(std::istream &is, RGBColor &color) {
  float r, g, b;
  if (!(is >> r >> g >> b)) {
    is.setstate(std::ios::failbit);
    return is;
  }

  /// Qualquer valor > 1 indica escala 0-255
  if (r > 1.f || g > 1.f || b > 1.f) {
    r /= 255.f;
    g /= 255.f;
    b /= 255.f;
  }

  color.r_ = r;
  color.g_ = g;
  color.b_ = b;
  return is;
}

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners_{colors} {}

  RGBColor sample(float u, float v) const {
    RGBColor t1 = lerp(corners_[bl], corners_[br], u);
    RGBColor t2 = lerp(corners_[tl], corners_[tr], u);
    return lerp(t1, t2, v);
  }

private:
  std::array<RGBColor, 4> corners_;

  RGBColor lerp(const RGBColor &A, const RGBColor &B, float t) const {
    return A * (1.0f - t) + B * t;
  }

  enum corners_e { bl = 0, tl, tr, br };
};

#endif
