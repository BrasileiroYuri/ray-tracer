#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "material.hpp"
#include "ray.hpp"
#include <memory>

struct Primitive {
  struct Surfel {
    std::shared_ptr<Material> mat_;
    float t_hit;
  };

  virtual ~Primitive() = default;

  // t_hit armazenará a distância da interseção mais próxima
  virtual bool intersect(const Ray &r, Surfel &s) const = 0;

  virtual Material *getMaterial() const = 0;
};

#endif
