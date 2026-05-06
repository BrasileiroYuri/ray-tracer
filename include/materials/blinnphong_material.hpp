#ifndef BLINNPHONG_MATERIAL_HPP
#define BLINNPHONG_MATERIAL_HPP

#include "background.hpp"
#include "material.hpp"
#include <algorithm> // std::max
#include <cmath>     // std::pow

class BlinnPhongMaterial : public Material {
public:
    RGBColor ka;
    RGBColor kd;
    RGBColor ks;
    float glossiness;

    BlinnPhongMaterial(const RGBColor& a, const RGBColor& d, const RGBColor& s, float g)
        : ka(a), kd(d), ks(s), glossiness(g) {}

    // Implementação da BRDF de Blinn-Phong
    RGBColor scatter(const vec3& wo, const vec3& wi, const vec3& n) const override {
        // 1. Componente Difusa (Lambertiana): cos(theta) * kd
        float cos_theta = std::max(0.0f, dot(n, wi));
        RGBColor diffuse = kd * cos_theta;

        // 2. Componente Especular: cos(alpha)^glossiness * ks
        vec3 h = normalize(wi + wo); // Vetor médio entre luz e câmera
        float cos_alpha = std::max(0.0f, dot(n, h));
        RGBColor specular = ks * std::pow(cos_alpha, glossiness);

        return diffuse + specular;
    }

    RGBColor getColor() const override { return kd; }
};

#endif // !BLINNPHONG_MATERIAL_HPP
