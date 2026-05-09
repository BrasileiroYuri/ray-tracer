#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
#include "light.hpp"
#include "math.hpp"
#include "scene.hpp"
#include <iostream>

class BlinnPhongIntegrator : public Integrator {
public:
  BlinnPhongIntegrator() = default;

  std::optional<RGBColor> li(const Ray &ray, const Scene &sc,
                             std::size_t depth) override {

    Surfel surfel;

    //  se não houver intersecção, o Integrator base usa o background
    if (!sc.aggregate_->intersect(ray, surfel)) {
      return {};
    }

    RGBColor L_total(0, 0, 0);

    vec3 wo = normalize(ray.direction_ * -1.0f);
    vec3 n = normalize(surfel.n);

    point3 offset_p = surfel.p + n * 1e-2f;
    for (const auto &light : sc.lights_) {
      vec3 wi;
      float dist;

      RGBColor intensity = light->sample_Li(surfel.p, wi, dist);

      // Ambient não tem direção — não precisa de shadow ray
      if (light->flags == LightFlag::Ambient) {
        L_total += intensity * surfel.mat_->getColor();
        continue;
      }

      Ray shadow(offset_p, wi, 0, dist);
      Surfel s;

      if (sc.aggregate_->intersect(shadow, s))
        continue; // em sombra — sem contribuição

      L_total += intensity * surfel.mat_->scatter(wo, wi, n);
    }
    if (depth < max_depth_) {
      auto dir = ray.direction_ - n * 2 * dot(ray.direction_, n);
      Ray reflected_ray(offset_p, normalize(dir), ray.min_t_, ray.max_t_);
      L_total += surfel.mat_->getMirror() *
                 li(reflected_ray, sc, depth + 1).value_or({0, 0, 0});
    }
    return std::make_optional(L_total);
  }
};

#endif // !BLINNPHONG_INTEGRATOR_HPP
