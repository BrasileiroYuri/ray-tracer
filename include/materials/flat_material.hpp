#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

#include "background.hpp"
#include "material.hpp"

class FlatMaterial : public Material {
public:
  FlatMaterial(RGBColor color) : color_(color) {}

  RGBColor getColor() const { return color_; }

private:
  RGBColor color_;
};
#endif // !FLAT_MATERIAL_HPP
