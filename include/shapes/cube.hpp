#ifndef CUBE_HPP
#define CUBE_HPP

#include "math.hpp"
#include "shape.hpp"
#include <algorithm>

class Cube : public Shape {
  struct Pair {
    float min, max;
  };

public:
  Cube(const point3 &p1, const point3 &p2) : p1_{p1}, p2_{p2} {}

  bool intersect(const Ray &r, Surfel &s) const {

    point3 min, max;
    min.i_ = std::min(p1_.i_, p2_.i_);
    min.j_ = std::min(p1_.j_, p2_.j_);
    min.k_ = std::min(p1_.k_, p2_.k_);

    max.i_ = std::max(p1_.i_, p2_.i_);
    max.j_ = std::max(p1_.j_, p2_.j_);
    max.k_ = std::max(p1_.k_, p2_.k_);

    auto ip = limits(min.i_, max.i_, r.origin_.i_, r.direction_.i_);
    auto jp = limits(min.j_, max.j_, r.origin_.j_, r.direction_.j_);
    auto kp = limits(min.k_, max.k_, r.origin_.k_, r.direction_.k_);

    auto tnear = std::max(ip.min, std::max(jp.min, kp.min));

    auto tfar = std::min(ip.max, std::min(jp.max, kp.max));

    if (tnear > tfar || tfar < 0.0f)
      return false;

    s.t_hit = (tnear >= 0.0f) ? tnear : tfar;
    s.p = r(s.t_hit);

    s.n.i_ = 0.0f;
    s.n.j_ = 0.0f;
    s.n.k_ = 0.0f;

    if (s.t_hit == ip.min || s.t_hit == ip.max) {
      s.n.i_ = (r.direction_.i_ < 0.0f) ? 1.0f : -1.0f;
    } else if (s.t_hit == jp.min || s.t_hit == jp.max) {
      s.n.j_ = (r.direction_.j_ < 0.0f) ? 1.0f : -1.0f;
    } else {
      s.n.k_ = (r.direction_.k_ < 0.0f) ? 1.0f : -1.0f;
    }

    return true;
  }
  Pair limits(float min, float max, float orig, float dir) const {

    auto t1 = (min - orig) / dir;
    auto t2 = (max - orig) / dir;

    auto tmin = std::min(t1, t2);
    auto tmax = std::max(t1, t2);

    return {tmin, tmax};
  }

private:
  point3 p1_;
  point3 p2_;
};
#endif // !CUBE_HPP
