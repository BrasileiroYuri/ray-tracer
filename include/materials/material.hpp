#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "background.hpp"
#include "math.hpp" // Para acessar vec3 e funções como dot()

class Material {
public:
    virtual ~Material() = default;


    virtual RGBColor scatter(const vec3& wo, const vec3& wi, const vec3& n) const = 0;

    /* Mantido para compatibilidade (ex: luz ambiente ou materiais sólidos). */
    virtual RGBColor getColor() const = 0; 
};

#endif