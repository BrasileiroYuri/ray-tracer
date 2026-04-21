#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "background.hpp"
class Material {
public:
  virtual ~Material() = default;

  /* Futuramente será 'scatter'. */
  virtual RGBColor getColor() const = 0;
};

#endif // !MATERIAL_HPP
