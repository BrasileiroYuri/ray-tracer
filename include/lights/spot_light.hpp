#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "light.hpp"

struct SpotLight : public Light {
  SpotLight(const RGBColor &i, const RGBColor &s, const point3 &from,const point3 &to, std::size_t c, std::size_t f) : Light(LightFlag::Spot, i, s), cutoff(c), falloff(f), dir(normalize(from-to)), pos(from) {}

  RGBColor sample_Li(const point3& hit_p, vec3& wi, float& dist) override {
    auto vec = hit_p - pos;
    dist = size(vec); 
    wi = normalize(vec); 
    float angle = dot(wi, dir*-1); /// Ângulo entre o vetor p -> l e direção da luz. 
    if (angle <= cutoff) {
      if (angle <= falloff) 
        return intensity * scale;
      else 
        return intensity * scale; // lerp between?
    }

      return {0,0,0};
  }

private:
  std::size_t cutoff;
  std::size_t falloff;
  point3 pos;
  vec3 dir;
};


#endif //! SPOTLIGHT_HPP
