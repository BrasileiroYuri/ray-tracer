#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "material.hpp"
#include "ray.hpp"

class Primitive {
public:
  virtual ~Primitive() = default;
  // t_hit armazenará a distância da interseção mais próxima
  virtual bool intersect(const Ray &r, float &t_hit) const = 0;
  virtual Material *getMaterial() const = 0;
};

#endif // !PRIMITIVE_HPP
