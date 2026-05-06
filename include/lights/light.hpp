#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "background.hpp"
#include "math.hpp"
#include "parser.hpp"
#include <cstdint> // Define uint8_t

enum class LightFlag : uint8_t {
    Ambient = 1,
    Directional = 2,
    Point = 4
};

class Light {
public:
    LightFlag flags;
    RGBColor intensity;
    RGBColor scale;

    Light(LightFlag f, RGBColor i, RGBColor s) : flags(f), intensity(i), scale(s) {}
    virtual ~Light() = default;

    // Retorna a cor da luz no ponto, a direção wi (ponto -> luz) e a distância
    virtual RGBColor sample_Li(const point3& hit_p, vec3& wi, float& dist) = 0;
};

#endif