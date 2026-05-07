#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "integrator.hpp"

struct RayCastIntegrator : public Integrator {
  std::optional<RGBColor> li(const Ray &ray, const Scene &sc) override {
    Surfel s;

    // Apenas retorna a cor intrínseca do material no ponto de impacto
    if (sc.aggregate_->intersect(ray, s))
      return std::make_optional<RGBColor>(s.mat_->scatter(NULL, NULL, NULL));

    return {};
  }
};
#endif
