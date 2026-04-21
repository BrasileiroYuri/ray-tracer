#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "integrator.hpp"

struct RayCastIntegrator : public Integrator {
  virtual void render(Scene sc, RenderConfig rc) override {

    auto widht = cam_->widht();
    auto height = cam_->height();

    for (std::size_t j = 0; j < height; j++) {
      for (std::size_t i = 0; i < widht; i++) {
      }
    }
  }
};
#endif // !RAYCAST_INTEGRATOR_HPP
