#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "background.hpp"
#include "math.hpp" // Para acessar vec3 e funções como dot()

struct Material {
    virtual ~Material() = default;
    virtual RGBColor scatter(const vec3& wo, const vec3& wi, const vec3& n) const = 0;
    virtual RGBColor getColor() const = 0;
};

#endif
