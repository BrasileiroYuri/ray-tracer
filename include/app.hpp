#ifndef API_HPP
#define API_HPP

#include "background.hpp"
#include "camera.hpp"
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
  static void backGround(ParamSet &);
  static void film(ParamSet &);
  static void lookat(ParamSet &);
  static void camera(ParamSet &);
  static void sphere(ParamSet &);
  static void object(ParamSet &);
  static void integrator(ParamSet &);

private:
  static void calculateScreenWindow();
  static void write_image();
  static std::unique_ptr<Camera> camera_;
  static BackGroundColor background_;
  static std::string filename_;
  static bool ppm_;
  static std::vector<std::unique_ptr<Primitive>> primitives_;
};

#endif // !API_HPP
