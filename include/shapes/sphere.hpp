#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include <cmath>

class Sphere : public Shape {
public:
  // Construtor atualizado para suportar esferas parciais
  Sphere(point3 center, float radius, float z_min = -1e6f, float z_max = 1e6f,
         float phi_max = 360.0f)
      : center_(center), radius_(radius), z_min_(z_min), z_max_(z_max),
        phi_max_(phi_max * M_PI / 180.0f) {

    float low_z = std::max(-radius, z_min);
    float high_z = std::min(radius, z_max);

    theta_min_ = std::acos(high_z / radius);
    theta_max_ = std::acos(low_z / radius);
  }

  bool intersect(const Ray &r, float &t_hit) const override;

private:
  point3 center_;
  float radius_;
  float z_min_, z_max_;
  float theta_min_, theta_max_, phi_max_;
};

#endif
