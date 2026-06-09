#ifndef AGGREGATE_PRIMITIVE_HPP
#define AGGREGATE_PRIMITIVE_HPP

#include "material.hpp"
#include "primitive.hpp"
#include <memory>

class AggregatePrimitive : public Primitive {
public:
  virtual void addObject(std::shared_ptr<Primitive> p) = 0;
  // Usado para construir a BVH e no PrimList faz nada
  virtual void build() {}
  Material *getMaterial() const override { return nullptr; }
};

#endif
