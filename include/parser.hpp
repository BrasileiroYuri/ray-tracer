#ifndef PARSER_HPP
#define PARSER_HPP

#include "api.hpp"
#include "param_set.hpp"

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>

class Parser {
public:
  Parser(const std::string &filename) : filename_(filename) {}
  void parse() const;

private:
  std::unordered_map<std::string, std::function<void(ParamSet)>> elements_{
      {"film", Api::film}, {"background", Api::backGround}};

  template <typename T>
  static void convert(const std::string &, const std::string &, ParamSet *);

  template <typename T, std::size_t size>
  static void convert(const std::string &, const std::string &, ParamSet *);

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
          {"bl", convert<RGBColor, 3>},
          {"br", convert<RGBColor, 3>},
          {"tl", convert<RGBColor, 3>},
          {"tr", convert<RGBColor, 3>},
      };

  std::string filename_;
};

#endif // !PARSER_HPP
