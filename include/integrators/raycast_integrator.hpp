#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "background.hpp"
#include "integrator.hpp"
#include "scene.hpp"

struct RayCastIntegrator : public Integrator {
  std::optional<RGBColor> li(const Ray &ray, const Scene &sc) override {

    Surfel s;

    if (sc.aggregate_->intersect(ray, s))
      return s.mat_->getColor();

    return {};
  }
};
#endif // !RAYCAST_INTEGRATOR_HPP. 
