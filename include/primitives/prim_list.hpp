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
    float closest = s.t_hit;

    for (const auto &p : primitives_) {
      float t = closest;
      if (p->intersect(r, s)) {
        hit = true;
        if (t < closest) {
          closest = t;
        }
      }
    }

    s.t_hit = closest;
    return hit;
  }

private:
  std::vector<std::shared_ptr<Primitive>> primitives_;
};

#endif
