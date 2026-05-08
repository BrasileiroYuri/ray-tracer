#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

#include "background.hpp"
#include "material.hpp"

class FlatMaterial : public Material {
public:
    FlatMaterial(RGBColor color) : color_(color) {}

    // Materiais flat ignoram vetores de luz e normal
    RGBColor scatter(const vec3& wo, const vec3& wi, const vec3& n) const override {
        return color_;
    }

  RGBColor getColor() const override {return color_;}
  RGBColor getMirror() const override {return {0,0,0};} // Espelhamento nulo.
private:
    RGBColor color_;
};
#endif
