#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "integrator.hpp"

struct RayCastIntegrator : public Integrator {
  std::optional<RGBColor> li(const Ray &ray, const Scene &sc) override {
    Surfel s;

    // Apenas retorna a cor intrínseca do material no ponto de impacto
    if (sc.aggregate_->intersect(ray, s))
      return s.mat_->getColor();

    return {};
  }
};
#endif