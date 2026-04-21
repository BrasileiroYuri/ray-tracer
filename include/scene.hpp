#ifndef SCENE_HPP
#define SCENE_HPP

#include "aggregate_primitive.hpp"
#include "background.hpp"
#include "prim_list.hpp"
#include "primitive.hpp"

#include <memory>

class Scene {
public:
private:
  std::unique_ptr<BackGroundColor> backGroundColor_;
  std::unique_ptr<AggregatePrimitive> primitive_ = std::make_unique<PrimList>();
};
#endif // !SCENE_HPP
