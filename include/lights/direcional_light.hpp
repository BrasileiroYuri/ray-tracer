#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "light.hpp"
#include <limits>

class DirectionalLight : public Light {
public:
    vec3 dir;

    DirectionalLight(const RGBColor& i, const RGBColor& s, const point3& from, const point3& to)
        : Light(LightFlag::Directional, i, s) {
        dir = normalize(from - to); // Direção para onde a luz aponta
    }

    RGBColor sample_Li(const point3& hit_p, vec3& wi, float& dist) override {
        wi = dir;
        dist = std::numeric_limits<float>::max();
        return intensity * scale;
    }
};

#endif
