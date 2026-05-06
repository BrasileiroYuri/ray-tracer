#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "primitive.hpp"

struct Shape {
  virtual ~Shape() = default;
  virtual bool intersect(const Ray &r, Surfel &s) const = 0;
};

#endif // !SHAPE_HPP
