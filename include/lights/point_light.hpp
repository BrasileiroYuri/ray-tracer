#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"
#include <cmath> 

class PointLight : public Light {
public:
    point3 pos;

    PointLight(const RGBColor& i, const RGBColor& s, const point3& from) 
        : Light(LightFlag::Point, i, s), pos(from) {}

    RGBColor sample_Li(const point3& hit_p, vec3& wi, float& dist) override {
        // Vetor que aponta do ponto de impacto para a posição da luz
        vec3 light_vec = pos - hit_p; 
        
        dist = std::sqrt(dot(light_vec, light_vec)); 
        
        if (dist > 0) {
            wi = light_vec * (1.0f / dist); 
        } else {
            wi = vec3(0, 0, 0);
        }

        return intensity * scale; 
    }
};

#endif