#ifndef RAY_HPP
#define RAY_HPP

#include "math.hpp"
#include <limits>
#include <sstream>
#include <string>

class Ray {
public:
  Ray() : min_t_{0.001f}, max_t_{std::numeric_limits<float>::infinity()} {}

  // Construtor com origem, direção e suporte opcional a limites de t
  Ray(const point3 &origin, const vec3 &direction, float t_min = 0.001f,
      float t_max = std::numeric_limits<float>::infinity())
      : origin_{origin}, direction_{direction}, min_t_{t_min}, max_t_{t_max} {}

  point3 origin() const { return origin_; }
  vec3 direction() const { return direction_; }
  float t_min() const { return min_t_; }
  float t_max() const { return max_t_; }

  // Setter para atualizar o t máximo durante a busca pela primeira interseção
  void set_t_max(float t) { max_t_ = t; }

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
