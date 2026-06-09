#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "bounds3f.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <cmath>
#include <memory>

class material;
class Bounds3f;

struct Surfel {
  std::shared_ptr<Material> mat_;
  float t_hit = INFINITY;
  point3 p;    // Ponto de interseção no espaço 3D
  vec3 n;      // Normal de shading (interpolada por vértice — suavizada)
  vec3 geom_n; // Normal geométrica (constante por triângulo — usada para shadow
               // bias)
  point2 uv;   // Coordenadas de textura interpoladas
};

struct Primitive {

  virtual ~Primitive() = default;

  // Surfel.t_hit armazenará a distância da interseção mais próxima
  virtual bool intersect(const Ray &r, Surfel &s) const = 0;

  // Returns the world-space AABB of this primitive.
  // Default: returns false (e.g. infinite planes have no finite bounding box).
  virtual bool world_bound(Bounds3f &box) const {
    (void)box;
    return false;
  }

  virtual Material *getMaterial() const = 0;
};

#endif // !PRIMITIVE_HPP
