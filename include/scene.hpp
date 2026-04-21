#ifndef SCENE_HPP
#define SCENE_HPP

#include "aggregate_primitive.hpp"
#include "background.hpp"
#include "primitive.hpp"

#include <array>
#include <memory>

class Scene {
public:
  Scene(const std::array<RGBColor, 4> &arr,
        std::unique_ptr<AggregatePrimitive> ag)
      : backGroundColor_(arr), aggregate_(std::move(ag)) {}

  BackGroundColor backGroundColor_;
  std::unique_ptr<AggregatePrimitive> aggregate_;
};
#endif // !SCENE_HPP
