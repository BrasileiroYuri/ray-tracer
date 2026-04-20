#ifndef RAYCAST_INTEGRATOR_HPP
#define RAYCAST_INTEGRATOR_HPP

#include "camera.hpp"
#include "integrator.hpp"

class RayCastIntegrator : public Integrator {

public:
  virtual void render(Scene) override;

private:
  static std::unique_ptr<Camera> camera_;
};
#endif // !RAYCAST_INTEGRATOR_HPP
