#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
class BlinnPhongIntegrator : public Integrator {
  std::optional<RGBColor> li(const Ray &ray, const Scene &sc) {}
};

#endif // !BLINNPHONG_INTEGRATOR_HPP
