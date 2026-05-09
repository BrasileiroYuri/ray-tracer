#include "math.hpp"
#include "shape.hpp"
#ifndef PLANE_HPP

class Plane : public Shape {
public:
  Plane(point3 p, vec3 n) : p_{p}, n_{n} {}
  bool intersect(const Ray &r, Surfel &s) const {

    auto t = dot(n_, (p_ - r.origin_)) / dot(n_, r.direction_);
    if (t < r.min_t_ || t > r.max_t_)
      return false;
    else
      s.t_hit = t;

    s.p = r(t);
    s.n = n_;

    return true;
  }

private:
  point3 p_;
  vec3 n_;
};
#endif // !PLANE_HPP
