#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "primitive.hpp"
#include "shape.hpp"
#include <iostream>
#include <memory>

class GeometricPrimitive : public Primitive {
public:
  GeometricPrimitive(std::unique_ptr<Shape> shape,
                     std::shared_ptr<Material> material)
      : shape_(std::move(shape)), material_(std::move(material)) {

    auto c = material_->getColor();
    std::cout << "COLOR FROM GP ->" << (int)c.R << " " << (int)c.G << " "
              << (int)c.B << "\n";
  }

  bool intersect(const Ray &r, float &t_hit) const override {
    return shape_->intersect(r, t_hit);
  }

  Material *getMaterial() const override { return material_.get(); }

private:
  std::unique_ptr<Shape> shape_;
  std::shared_ptr<Material> material_;
};
#endif
