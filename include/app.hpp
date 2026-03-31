#ifndef API_HPP
#define API_HPP

#include "background.hpp"
#include "camera.hpp"
#include "param_set.hpp"
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

private:
  static void calculateScreenWindow();
  static void write_image();
  static std::unique_ptr<Camera> camera_;
  static BackGroundColor background_;
  static std::string filename_;
  static bool ppm_;
};

#endif // !API_HPP
