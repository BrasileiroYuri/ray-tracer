#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "light.hpp"
#include <cmath>

struct SpotLight : public Light {
  SpotLight(const RGBColor &i, const RGBColor &s, const point3 &from,
            const point3 &to, std::size_t c, std::size_t f)
      : Light(LightFlag::Spot, i, s),
        cutoff(std::cos(c * M_PI / 180.f)), // cos(30°) = 0.866
        falloff(std::cos(f * M_PI / 180.f)), dir(normalize(from - to)),
        pos(from) {}

  RGBColor sample_Li(const point3 &hit_p, vec3 &wi, float &dist) override {
    auto vec = hit_p - pos; /// Vetor da luz para o ponto.
    dist = size(vec);
    wi = normalize(vec) * -1; /// Como a função sample_Li do Light define wi =
                              /// ponto -> luz, retornei nesse sentido.

    float cosAngle = dot(wi, dir); // já é o cosseno

    /// poderia ter deixado em uma condição só, mas pra ficar mais legível vou
    /// deixar assim msm
    if (cosAngle >= cutoff) { // >= porque cosseno é invertido
      if (cosAngle >= falloff) {
        return intensity * scale;
      } else {
        float t = (cosAngle - cutoff) / (falloff - cutoff);
        return intensity * scale * t;
      }
    }
    return {0, 0, 0};
  }

private:
  float cutoff;
  float falloff;
  point3 pos;
  vec3 dir;
};

#endif //! SPOTLIGHT_HPP
