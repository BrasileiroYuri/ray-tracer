#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "integrator.hpp"

struct RayCastIntegrator : public Integrator {
  virtual void render(const Scene &sc, RenderConfig rc) override {

    auto widht = cam_->widht();
    auto height = cam_->height();

    for (std::size_t j = 0; j < height; j++) {
      for (std::size_t i = 0; i < widht; i++) {
        Ray ray = cam_->generateRay(i, j);

        float t_hit;
        if (sc.aggregate_->intersect(ray, t_hit)) {
          cam_->add(i, j, RGBColor(255, 0, 0));
        } else {

          float u = float(i) / float(widht - 1);
          float v = 1.f - (float(j) / (float)(height - 1));
          cam_->add(i, j, sc.backGroundColor_.sample(u, v));
        }
      }
    }
  }
};
#endif // !RAYCAST_INTEGRATOR_HPP
