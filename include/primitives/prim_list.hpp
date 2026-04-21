#ifndef PRIM_LIST_HPP
#define PRIM_LIST_HPP

#include "aggregate_primitive.hpp"
#include <memory>
#include <vector>

class PrimList : public AggregatePrimitive {
public:
  void addObject(std::unique_ptr<Primitive> p) override {
    primitives_.push_back(std::move(p));
  }

  bool intersect(const Ray &r, float &t_hit) const override {
    bool hit = false;
    float closest = t_hit;

    for (const auto &p : primitives_) {
      float t = closest;
      if (p->intersect(r, t)) {
        hit = true;
        if (t < closest) {
          closest = t;
        }
      }
    }

    t_hit = closest;
    return hit;
  }

private:
  std::vector<std::unique_ptr<Primitive>> primitives_;
};

#endif
