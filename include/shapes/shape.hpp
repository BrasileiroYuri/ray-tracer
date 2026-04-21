#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
struct Shape {
  virtual ~Shape() = default;
  virtual bool intersect(const Ray &r, float &t_hit) const = 0;
};

#endif // !SHAPE_HPP
