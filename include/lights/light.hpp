#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "background.hpp"
#include "math.hpp"
#include <cstdint> // Define uint8_t

enum class LightFlag : uint8_t { Ambient = 1, Directional, Point, Spot };

struct Light {
  LightFlag flags;
  RGBColor intensity;
  vec3 scale;

  Light(LightFlag f, RGBColor i, vec3 s) : flags(f), intensity(i), scale(s) {}
  virtual ~Light() = default;

  // Retorna a cor da luz no ponto, a direção wi (ponto -> luz) e a distância
  virtual RGBColor sample_Li(const point3 &hit_p, vec3 &wi, float &dist) = 0;
};

#endif
