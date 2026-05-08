#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
#include "light.hpp"
#include "math.hpp"
#include "scene.hpp"
#include <limits>

class BlinnPhongIntegrator : public Integrator {
public:
  BlinnPhongIntegrator() = default;

  std::optional<RGBColor> li(const Ray &ray, const Scene &sc, std::size_t depth) override {
    Surfel surfel;

    //  se não houver intersecção, o Integrator base usa o background
    if (!sc.aggregate_->intersect(ray, surfel)) {
      return {};
    }

    RGBColor L_total(0, 0, 0);

    vec3 wo = normalize(ray.direction() * -1.0f);
    vec3 n = normalize(surfel.n);

    //
    for (const auto &light : sc.lights_) {
      vec3 wi; // Direção da luz que será preenchida pelo sample_Li
      float dist;

      // Obtém a intensidade e a direção da luz (wi) para o ponto atual
      RGBColor intensity = light->sample_Li(surfel.p, wi, dist);

      /*
       * Origem definida igual ao ponto de intersecção,
       * Direção definida do ponto a luz,
       * Distância básica 0 e máxima igual dist.
       */

      point3 offset(
          0.1, 0.1,
          0.1); /// Offset usado para evitar erros de colisão (aka sarampo).

      Ray shadow(surfel.p + offset, wi, 1, dist);
      Surfel s;

      if (sc.aggregate_->intersect(shadow, s)) {
        if (light->flags == LightFlag::Ambient)
          L_total += intensity * s.mat_->getColor();
	surfel = s;
	
      } else {

      if (light->flags == LightFlag::Ambient)
        L_total += intensity * surfel.mat_->getColor();
      else
        L_total += intensity * surfel.mat_->scatter(wo, wi, n);
      }

      ///
      	auto dir = ray.direction_ - n * 2*dot(ray.direction_,n);
	Ray reflected_ray(surfel.p+offset, dir, ray.min_t_, ray.max_t_);
	RGBColor color(1,1,1);
	
    if (depth < max_depth_)
	    L_total+= surfel.mat_->getMirror() * li(reflected_ray, sc, depth+1).value_or(color);
    }

    return std::make_optional(L_total);
  }
};

#endif // !BLINNPHONG_INTEGRATOR_HPP
