#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
#include "light.hpp"
#include "math.hpp"
#include "scene.hpp"

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

    RGBColor L_total;

    vec3 wo = normalize(ray.direction_ * -1.0f);
    vec3 n =
        normalize(surfel.n); // shading normal — usada na equação de iluminação
    vec3 geom_n =
        normalize(surfel.geom_n); // normal geométrica — usada para o bias

    // Bias com geom_n: constante por triângulo, nunca aponta para dentro da
    // malha
    const float bias = 1e-2f;
    point3 offset_p = surfel.p + geom_n * bias;
    for (const auto &light : sc.lights_) {
      vec3 wi;
      float dist;

      RGBColor intensity = light->sample_Li(surfel.p, wi, dist);

      // Ambient não tem direção — não precisa de shadow ray
      if (light->flags == LightFlag::Ambient) {
        L_total += intensity * surfel.mat_->getColor();
        continue;
      }

      // min_t pequeno: offset_p já está bias acima da superfície
      Ray shadow(offset_p, wi, bias, dist);
      Surfel s;

      if (sc.aggregate_->intersect(shadow, s))
        continue; // em sombra — sem contribuição

      L_total += intensity * surfel.mat_->scatter(wo, wi, n);
    }
    if (depth < max_depth_) {
      auto dir = ray.direction_ - n * 2 * dot(ray.direction_, n);
      // min_t da câmera primária (0.001) evita auto-intersecção no raio
      // refletido
      Ray reflected_ray(offset_p, normalize(dir), bias, ray.max_t_);
      L_total += surfel.mat_->getMirror() *
                 li(reflected_ray, sc, depth + 1).value_or((RGBColor){0, 0, 0});
    }
    return std::make_optional(L_total);
  }
};

#endif // !BLINNPHONG_INTEGRATOR_HPP
