#ifndef API_HPP
#define API_HPP

#include "aggregate_primitive.hpp"
#include "integrator.hpp"
#include "param_set.hpp"
#include <memory>
#include <string>

struct ScreenWindow {
  ScreenWindow(float l, float r, float b, float t)
      : l_(l), r_(r), b_(b), t_(t) {}
  ScreenWindow(const std::array<float, 4> &arr)
      : l_{arr[0]}, r_{arr[1]}, b_{arr[2]}, t_{arr[3]} {}
  float l_, r_, b_, t_;
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
  static void include(const ParamSet &);
  static void named_material(const ParamSet &);

private:
  static void integratorConfig(const std::string &type);
  static std::unique_ptr<Integrator> integrator_;
  // static std::unique_ptr<AggregatePrimitive> aggrPrim;
};

#endif // !API_HPP
