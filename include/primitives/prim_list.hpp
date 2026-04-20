#ifndef PRIM_LIST_HPP
#define PRIM_LIST_HPP

#include "aggregate_primitive.hpp"
#include <memory>
#include <vector>

class PrimList : public AggregatePrimitive {

private:
  std::vector<std::unique_ptr<Primitive>> primitives_;
};
#endif // !PRIM_LIST_HPP
