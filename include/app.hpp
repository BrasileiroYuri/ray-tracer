#ifndef API_HPP
#define API_HPP

#include "background.hpp"
#include "camera.hpp"
#include "integrator.hpp"
#include "param_set.hpp"
#include "primitive.hpp"
#include <memory>
#include <string>

struct ScreenWindow {
  ScreenWindow() {}
  float l_, r_, b_, t_;
  ScreenWindow(const std::array<float, 4> &arr)
      : l_{arr[0]}, r_{arr[1]}, b_{arr[2]}, t_{arr[3]} {}
};

class App {
public:
  static void render();
  static void backGround(const ParamSet &);
  static void film(const ParamSet &);
  static void lookat(const ParamSet &);
  static void camera(const ParamSet &);
  static void sphere(const ParamSet &);
  static void object(const ParamSet &);
  static void integrator(const ParamSet &);
  static void make_named_material(const ParamSet &);
  static void material(const ParamSet &);
  static void named_material(const ParamSet &);

private:
  static std::unique_ptr<Integrator> integrator_;
  static Scene Scene_;
  static void calculateScreenWindow();
  static void write_image();
  static std::unique_ptr<Camera> camera_; // Vai ser substituido por Integrator
  static BackGroundColor background_;     // Vai ser substituido por Scene
  static std::string filename_;
  static bool ppm_;
  static std::vector<std::unique_ptr<Primitive>> primitives_;
};

#endif // !API_HPP
