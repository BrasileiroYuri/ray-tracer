#ifndef SCENE_HPP
#define SCENE_HPP

#include "background.hpp"
#include "primitive.hpp"
#include <memory>

class Scene {
public:
private:
  std::unique_ptr<BackGroundColor> backGroundColor_;
  std::unique_ptr<Primitive> primitive_;
};
#endif // !SCENE_HPP
