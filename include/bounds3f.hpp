#ifndef BOUNDS3F_HPP
#define BOUNDS3F_HPP

#include "math.hpp"
#include "ray.hpp"
#include <algorithm>
#include <limits>

// Axis-Aligned Bounding Box (AABB) in 3D.
// Used by BVHAccel to quickly discard rays that can't hit a subtree.
class Bounds3f {
public:
  point3 p_min, p_max;

  // Default: invalid/empty box (inverted extents).
  Bounds3f()
      : p_min{std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max()},
        p_max{std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest()} {}

  // Construct from two corner points; min/max per component is taken
  // automatically, so order doesn't matter.
  Bounds3f(const point3 &a, const point3 &b)
      : p_min{std::min(a.i_, b.i_), std::min(a.j_, b.j_), std::min(a.k_, b.k_)},
        p_max{std::max(a.i_, b.i_), std::max(a.j_, b.j_),
              std::max(a.k_, b.k_)} {}

  // Geometric center of the box.
  point3 centroid() const {
    return point3{(p_min.i_ + p_max.i_) * 0.5f, (p_min.j_ + p_max.j_) * 0.5f,
                  (p_min.k_ + p_max.k_) * 0.5f};
  }

  // Return the smallest AABB containing both this box and b.
  Bounds3f unite(const Bounds3f &b) const {
    return Bounds3f(
        point3{std::min(p_min.i_, b.p_min.i_), std::min(p_min.j_, b.p_min.j_),
               std::min(p_min.k_, b.p_min.k_)},
        point3{std::max(p_max.i_, b.p_max.i_), std::max(p_max.j_, b.p_max.j_),
               std::max(p_max.k_, b.p_max.k_)});
  }

  // Return the smallest AABB containing this box and the point p.
  Bounds3f unite(const point3 &p) const {
    return Bounds3f(point3{std::min(p_min.i_, p.i_), std::min(p_min.j_, p.j_),
                           std::min(p_min.k_, p.k_)},
                    point3{std::max(p_max.i_, p.i_), std::max(p_max.j_, p.j_),
                           std::max(p_max.k_, p.k_)});
  }

  // Ray-AABB slab test (Andrew Kensler / Pixar version).
  // Returns true if the ray intersects the box in the valid t range.
  // t0_out / t1_out receive the entry and exit ray parameters.
  bool intersect_p(const Ray &r, float &t0_out, float &t1_out) const {
    float tmin = r.min_t_;
    float tmax = r.max_t_;

    // X slab
    {
      float invD = 1.0f / r.direction_.i_;
      float ta0 = (p_min.i_ - r.origin_.i_) * invD;
      float ta1 = (p_max.i_ - r.origin_.i_) * invD;
      if (invD < 0.0f)
        std::swap(ta0, ta1);
      tmin = ta0 > tmin ? ta0 : tmin;
      tmax = ta1 < tmax ? ta1 : tmax;
      if (tmax <= tmin)
        return false;
    }
    // Y slab
    {
      float invD = 1.0f / r.direction_.j_;
      float ta0 = (p_min.j_ - r.origin_.j_) * invD;
      float ta1 = (p_max.j_ - r.origin_.j_) * invD;
      if (invD < 0.0f)
        std::swap(ta0, ta1);
      tmin = ta0 > tmin ? ta0 : tmin;
      tmax = ta1 < tmax ? ta1 : tmax;
      if (tmax <= tmin)
        return false;
    }
    // Z slab
    {
      float invD = 1.0f / r.direction_.k_;
      float ta0 = (p_min.k_ - r.origin_.k_) * invD;
      float ta1 = (p_max.k_ - r.origin_.k_) * invD;
      if (invD < 0.0f)
        std::swap(ta0, ta1);
      tmin = ta0 > tmin ? ta0 : tmin;
      tmax = ta1 < tmax ? ta1 : tmax;
      if (tmax <= tmin)
        return false;
    }

    t0_out = tmin;
    t1_out = tmax;
    return true;
  }
};

#endif // !BOUNDS3F_HPP
