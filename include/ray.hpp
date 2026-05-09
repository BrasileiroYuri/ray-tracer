#ifndef RAY_HPP
#define RAY_HPP

#include "math.hpp"
#include <limits>
#include <sstream>
#include <string>

struct Ray {
  Ray() : min_t_{0.001f}, max_t_{std::numeric_limits<float>::infinity()} {}

  // Construtor com origem, direção e suporte opcional a limites de t
  Ray(const point3 &o, const vec3 &d, float min = 0.001f,
      float max = std::numeric_limits<float>::infinity())
      : origin_{o}, direction_{normalize(d)}, min_t_{min}, max_t_{max} {}

  // Operador para calcular o ponto na reta: p(t) = o + t*d
  point3 operator()(float t) const { return origin_ + direction_ * t; }

  std::string str() const {
    std::stringstream s;
    s << "[o=(" << origin_.i_ << ", " << origin_.j_ << ", " << origin_.k_
      << "), d=(" << direction_.i_ << ", " << direction_.j_ << ", "
      << direction_.k_ << "), t_range=[" << min_t_ << ", " << max_t_ << "]]";
    return s.str();
  };

  point3 origin_;
  vec3 direction_;
  float min_t_;
  float max_t_;
};

#endif // !RAY_HPP
