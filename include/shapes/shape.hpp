#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "primitive.hpp"
#include "ray.hpp"

struct Shape {
  virtual ~Shape() = default;
  virtual bool intersect(const Ray &r, Surfel &s) const = 0;
  // Returns the world-space AABB of this shape.
  // Default: no finite bounding box (e.g. infinite planes).
  virtual bool world_bound(Bounds3f &box) const {
    (void)box;
    return false;
  }
};

#endif // !SHAPE_HPP
