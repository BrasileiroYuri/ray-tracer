#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "material.hpp"
#include "primitive.hpp"
#include "shape.hpp"
#include <memory>

class GeometricPrimitive : public Primitive {

private:
  std::unique_ptr<Shape> shape_;
  std::unique_ptr<Material> material_;
};
#endif // !GEOMETRIC_PRIMITIVE_HPP
