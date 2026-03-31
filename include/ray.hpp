#ifndef RAY_HPP
#define RAY_HPP

#include "math.hpp"

class Ray {
public:
  Ray() {}
  Ray(const point3 &origin, const vec3 &direction)
      : origin_{origin}, direction_{direction} {}

private:
  // float t_min_, t_max_;
  point3 origin_;
  vec3 direction_;
};

#endif // !RAY_HPP
