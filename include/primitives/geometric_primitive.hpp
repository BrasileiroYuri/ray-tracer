#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "primitive.hpp"
#include "shape.hpp"
#include <memory>

class GeometricPrimitive : public Primitive {
public:
  GeometricPrimitive(std::unique_ptr<Shape> shape,
                     std::shared_ptr<Material> material)
      : shape_(std::move(shape)), material_(std::move(material)) {}

  bool intersect(const Ray &r, Surfel &s) const override {
    bool result = shape_->intersect(r, s.t_hit);
    s.mat_ = material_;
    return result;
  }

  Material *getMaterial() const override { return material_.get(); }

private:
  std::unique_ptr<Shape> shape_;
  std::shared_ptr<Material> material_;
};
#endif // !GEOMETRIC_PRIMITIVE_HPP
