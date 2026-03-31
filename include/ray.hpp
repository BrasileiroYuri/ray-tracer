#ifndef RAY_HPP
#define RAY_HPP

#include "math.hpp"
#include <sstream>
#include <string.h>

class Ray {
public:
  Ray() {}
  Ray(const point3 &origin, const vec3 &direction)
      : origin_{origin}, direction_{direction} {}

  std::string str() const {
    std::stringstream s;
    s << "[o=(" << origin_.i_ << ", " << origin_.j_ << ", " << origin_.k_
      << "), " << " d=(" << direction_.i_ << ", " << direction_.j_ << ", "
      << direction_.k_ << ")]";
    return s.str();
  };

private:
  // float t_min_, t_max_;
  point3 origin_;
  vec3 direction_;
};

#endif // !RAY_HPP
