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
      : shape_(std::move(shape)), material_(material) {

    auto c = material_->getColor();
    std::cout << "COLOR FROM GP ->" << (int)c.R << " " << (int)c.G << " "
              << (int)c.B << "\n";
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    bool result = shape_->intersect(r, s.t_hit);

    std::cout << "COLOR FROM INTERSECT: ->" << (int)material_->getColor().R
              << " " << (int)material_->getColor().G << " "
              << (int)material_->getColor().B << "\n";
    s.mat_ = material_;
    return result;
  }

  Material *getMaterial() const override { return material_.get(); }

private:
  std::unique_ptr<Shape> shape_;
  std::shared_ptr<Material> material_;
};
#endif
