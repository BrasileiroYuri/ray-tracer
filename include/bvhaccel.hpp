#ifndef BVHACCEL_HPP
#define BVHACCEL_HPP

#include "aggregate_primitive.hpp"
#include "bounds3f.hpp"
#include "primitive.hpp"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>

class BVHNode : public Primitive {
public:
  Bounds3f bounds;
  std::shared_ptr<Primitive> left, right;

  bool world_bound(Bounds3f &box) const override {
    box = bounds;
    return true;
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    float t0, t1;
    if (!bounds.intersect_p(r, t0, t1))
      return false;

    Surfel left_s, right_s;
    bool hit_left = left && left->intersect(r, left_s);
    bool hit_right = right && right->intersect(r, right_s);

    if (hit_left && hit_right) {
      s = (left_s.t_hit <= right_s.t_hit) ? left_s : right_s;
      return true;
    }
    if (hit_left) {
      s = left_s;
      return true;
    }
    if (hit_right) {
      s = right_s;
      return true;
    }
    return false;
  }

  Material *getMaterial() const override { return nullptr; }
};

inline std::shared_ptr<Primitive>
buildBVHNode(std::vector<std::shared_ptr<Primitive>> &prims, std::size_t start,
             std::size_t end) {
  std::size_t n = end - start;
  if (n == 0)
    return nullptr;

  auto node = std::make_shared<BVHNode>();

  if (n == 1) {
    node->left = node->right = prims[start];

  } else if (n == 2) {
    node->left = prims[start];
    node->right = prims[start + 1];

  } else {
    int axis = std::rand() % 3;

    std::sort(prims.begin() + static_cast<std::ptrdiff_t>(start),
              prims.begin() + static_cast<std::ptrdiff_t>(end),
              [axis](const std::shared_ptr<Primitive> &a,
                     const std::shared_ptr<Primitive> &b) {
                Bounds3f ba, bb;
                bool ok_a = a->world_bound(ba);
                bool ok_b = b->world_bound(bb);
                if (!ok_a || !ok_b)
                  return false;
                auto ca = ba.centroid();
                auto cb = bb.centroid();
                if (axis == 0)
                  return ca.i_ < cb.i_;
                if (axis == 1)
                  return ca.j_ < cb.j_;
                return ca.k_ < cb.k_;
              });

    std::size_t mid = start + n / 2;
    node->left = buildBVHNode(prims, start, mid);
    node->right = buildBVHNode(prims, mid, end);
  }

  Bounds3f bl, br;
  bool vl = node->left && node->left->world_bound(bl);
  bool vr = node->right && node->right->world_bound(br);
  if (vl && vr)
    node->bounds = bl.unite(br);
  else if (vl)
    node->bounds = bl;
  else if (vr)
    node->bounds = br;

  return node;
}

class BVHAccel : public AggregatePrimitive {
public:
  explicit BVHAccel(int maxPrimsPerNode = 1)
      : maxPrimsPerNode_(maxPrimsPerNode) {}

  void addObject(std::shared_ptr<Primitive> p) override {
    primitives_.push_back(std::move(p));
  }

  // Usado para construir a BVH. Tem que ser chamado antes de intersect.
  void build() override {
    if (primitives_.empty())
      return;

    root_ = buildBVHNode(primitives_, 0, primitives_.size());
  }

  bool world_bound(Bounds3f &box) const override {
    if (!root_)
      return false;
    return root_->world_bound(box);
  }

  bool intersect(const Ray &r, Surfel &s) const override {
    if (!root_)
      return false;
    return root_->intersect(r, s);
  }

private:
  const int maxPrimsPerNode_;
  std::vector<std::shared_ptr<Primitive>> primitives_;
  std::shared_ptr<Primitive> root_;
};
#endif //! BVHACCEL_HPP
