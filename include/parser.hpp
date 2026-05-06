#ifndef PARSER_HPP
#define PARSER_HPP

#include "param_set.hpp"
#include <functional>
#include <string>
#include <unordered_map>

class Parser {
public:
  Parser(const std::string &filename);
  void parse() const;

private:
  std::unordered_map<std::string, std::function<void(ParamSet &)>> elements_;

  /*
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
  }*/

  std::unordered_map<
      std::string,
      std::function<void(const std::string &, const std::string &, ParamSet *)>>
      conversor_;

  void include(const std::string &) const;
  std::string filename_;
};

#endif // !PARSER_HPP
