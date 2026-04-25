#ifndef PARSER_HPP
#define PARSER_HPP

#include "app.hpp"
#include "param_set.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

class Parser {
public:
  Parser(const std::string &filename) : filename_(filename) {}
  void parse() const;

private:
  std::unordered_map<std::string, std::function<void(ParamSet &)>> elements_{
      {"film", App::film},
      {"background", App::backGround},
      {"camera", App::camera},
      {"lookat", App::lookat},
      {"object", App::object},
      {"make_named_material", App::make_named_material},
      {"named_material", App::named_material},
      {"material", App::material},
      {"integrator", App::integrator}};

  template <typename T>
  static void convert(const std::string &name, const std::string &value,
                      ParamSet *ps) {
    std::istringstream ss{value};
    T val{};
    ss >> val;
    ps->add(name, val);
  }

  template <typename T, typename K, std::size_t size>
  static void convert(const std::string &name, const std::string &value,
                      ParamSet *ps) {
    std::istringstream ss{value};
    std::array<K, size> arr;
    for (unsigned long i = 0; i < size; i++)
      ss >> arr[i];
    T val{arr};
    ps->add(name, val);
  }

  std::unordered_map<
      std::string,
      std::function<void(const std::string &, const std::string &, ParamSet *)>>
      conversor_{
          {"frame_aspectratio", convert<float>},
          {"radius", convert<float>},
          {"z_min", convert<float>},
          {"z_max", convert<float>},
          {"phi_max", convert<float>},
          {"w_res", convert<int>},
          {"h_res", convert<int>},
          {"fovy", convert<int>},
          {"type", convert<std::string>},
          {"name", convert<std::string>},
          {"filename", convert<std::string>},
          {"img_type", convert<std::string>},
          {"mapping", convert<std::string>},
          {"bl", convert<RGBColor, int, 3>},
          {"br", convert<RGBColor, int, 3>},
          {"tl", convert<RGBColor, int, 3>},
          {"tr", convert<RGBColor, int, 3>},
          {"single_color", convert<RGBColor, int, 3>},
          {"color", convert<RGBColor, int, 3>},
          {"look_from", convert<point3, float, 3>},
          {"look_at", convert<point3, float, 3>},
          {"center", convert<point3, float, 3>},
          {"up", convert<vec3, float, 3>},
          {"screen_window", convert<ScreenWindow, float, 4>},
      };

  void include(const std::string &) const;
  std::string filename_;
};

#endif // !PARSER_HPP
