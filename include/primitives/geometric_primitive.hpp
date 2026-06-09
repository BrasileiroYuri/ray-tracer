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
    if (shape_->intersect(r, s)) {
      s.mat_ = material_;
      return true;
    }
    return false;
  }

  // Delegates bounding-box query to the underlying shape.
  bool world_bound(Bounds3f &box) const override {
    return shape_->world_bound(box);
  }

  Material *getMaterial() const override { return material_.get(); }

private:
  std::unique_ptr<Shape> shape_;
  std::shared_ptr<Material> material_;
};
#endif // !GEOMETRIC_PRIMITIVE_HPP
