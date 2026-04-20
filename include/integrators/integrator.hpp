#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "scene.hpp"

class Integrator {
public:
  virtual ~Integrator() = default;
  virtual void render(Scene) = 0;
};
#endif // !INTEGRATOR_HPP
