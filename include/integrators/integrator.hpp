#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "camera.hpp"
#include "orthographic_camera.hpp"
#include "perspective_camera.hpp"
#include "scene.hpp"
#include <memory>
#include <string>

#define Surfel Primitive::Surfel

struct RenderConfig {};

class Integrator {
public:
  virtual ~Integrator() = default;

  virtual void render(const Scene &, RenderConfig = RenderConfig{}) = 0;

  void makeCamera(CameraConfig cc) {
    if (cc.type == "perspective")
      cam_ = std::make_unique<Perspective>(cc);
    else
      cam_ = std::make_unique<Orthographic>(cc);
  }

  void write_image(std::string &f, bool ppm = false) const {
    cam_->write_image(f, ppm);
  }

protected:
  std::unique_ptr<Camera> cam_ = nullptr;
};
#endif // !INTEGRATOR_HPP
