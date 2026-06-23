#ifndef API_HPP
#define API_HPP

#include "integrator.hpp"
#include "param_set.hpp"
#include <istream>
#include <memory>
#include <string>

struct ScreenWindow {
  ScreenWindow() = default;
  float l_, r_, b_, t_;
};

inline std::istream &operator>>(std::istream &is, ScreenWindow &sc) {
  if (!(is >> sc.l_ >> sc.r_ >> sc.b_ >> sc.t_))
    is.setstate(std::ios::failbit);
  return is;
}
class App {
public:
  static void render();
  static void backGround(const ParamSet &);
  static void film(const ParamSet &);
  static void lookat(const ParamSet &);
  static void camera(const ParamSet &);
  static void object(const ParamSet &);
  static void integrator(const ParamSet &);
  static void make_named_material(const ParamSet &);
  static void material(const ParamSet &);
  static void named_material(const ParamSet &);
  static void light_source(const ParamSet &);
  static void aggregator(const ParamSet &);
  static void identity(const ParamSet &);
  static void translate(const ParamSet &);
  static void scale(const ParamSet &);
  static void rotate(const ParamSet &);
  static void save_coord_system(const ParamSet &);
  static void restore_coord_system(const ParamSet &);

private:
  static void plane(const ParamSet &);
  static void sphere(const ParamSet &);
  static void cube(const ParamSet &);
  static void pyramid(const ParamSet &); //

  static void integratorConfig(const std::string &);
  static std::unique_ptr<Integrator> integrator_;
};

#endif // !API_HPP
