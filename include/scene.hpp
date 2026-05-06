#ifndef SCENE_HPP
#define SCENE_HPP

#include "aggregate_primitive.hpp"
#include "background.hpp"
#include "light.hpp" 
#include <array>
#include <memory>
#include <vector>

class Scene {
public:
  Scene(const std::array<RGBColor, 4> &arr,
        std::unique_ptr<AggregatePrimitive> ag,
        std::vector<std::shared_ptr<Light>> lights)
      : backGroundColor_(arr), aggregate_(std::move(ag)), lights_(std::move(lights)) {}

  BackGroundColor backGroundColor_;
  std::unique_ptr<AggregatePrimitive> aggregate_;
  std::vector<std::shared_ptr<Light>> lights_; 
};

#endif // !SCENE_HPP