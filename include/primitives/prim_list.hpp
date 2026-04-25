#ifndef PRIM_LIST_HPP
#define PRIM_LIST_HPP

#include "aggregate_primitive.hpp"
#include "primitive.hpp"
#include <memory>
#include <vector>

class PrimList : public AggregatePrimitive {
public:
  void addObject(std::shared_ptr<Primitive> p) override {
    primitives_.push_back(p);
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    bool hit = false;
    float closest = std::numeric_limits<float>::max();
    Surfel closest_surfel;

    for (const auto &p : primitives_) {
      Surfel temp;
      temp.t_hit = closest;
      if (p->intersect(r, temp) && temp.t_hit < closest) {
        hit = true;
        closest = temp.t_hit;
        closest_surfel = temp;
      }
    }

    if (hit)
      s = closest_surfel;
    return hit;
  }

private:
  std::vector<std::shared_ptr<Primitive>> primitives_;
};

#endif
