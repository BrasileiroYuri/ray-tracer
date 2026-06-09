#ifndef AGGREGATE_PRIMITIVE_HPP
#define AGGREGATE_PRIMITIVE_HPP

#include "material.hpp"
#include "primitive.hpp"
#include <memory>

class AggregatePrimitive : public Primitive {
public:
  virtual void addObject(std::shared_ptr<Primitive> p) = 0;
  // Called once, after all objects are added, before rendering.
  // PrimList leaves this as a no-op; BVHAccel builds its tree here.
  virtual void build() {}
  Material *getMaterial() const override { return nullptr; }
};

#endif
