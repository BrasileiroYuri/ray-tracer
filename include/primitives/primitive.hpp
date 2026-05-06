#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "material.hpp"
#include "ray.hpp"
#include <cmath>
#include <memory>

class material;

struct Surfel {
  std::shared_ptr<Material> mat_;
  float t_hit = INFINITY;
  point3 p; // Ponto de interseção no espaço 3D
  vec3 n;   // Vetor normal da superfície no ponto p
};

struct Primitive {

  virtual ~Primitive() = default;

  // Surfel.t_hit armazenará a distância da interseção mais próxima
  virtual bool intersect(const Ray &r, Surfel &s) const = 0;

  virtual Material *getMaterial() const = 0;
};

#endif // !PRIMITIVE_HPP
