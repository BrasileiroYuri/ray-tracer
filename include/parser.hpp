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
  std::unordered_map<std::string, std::function<void(ParamSet)>> elements_{
      {"film", App::film}, {"background", App::backGround}};

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
          {"type", convert<std::string>},
          {"x_res", convert<int>},
          {"y_res", convert<int>},
          {"filename", convert<std::string>},
          {"img_type", convert<std::string>},
          {"mapping", convert<std::string>},
          {"bl", convert<RGBColor, int, 3>},
          {"br", convert<RGBColor, int, 3>},
          {"tl", convert<RGBColor, int, 3>},
          {"tr", convert<RGBColor, int, 3>},
          {"color", convert<RGBColor, int, 3>},
      };

  std::string filename_;
};

#endif // !PARSER_HPP
