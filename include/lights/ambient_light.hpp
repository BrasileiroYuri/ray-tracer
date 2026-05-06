#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "light.hpp"

class AmbientLight : public Light {
public:
    AmbientLight(const RGBColor& i, const RGBColor& s) 
        : Light(LightFlag::Ambient, i, s) {}

    RGBColor sample_Li(const point3& hit_p, vec3& wi, float& dist) override {
        wi = vec3(0, 0, 0);
        dist = 0;
        return intensity * scale; // Retorna intensidade constante
    }
};

#endif