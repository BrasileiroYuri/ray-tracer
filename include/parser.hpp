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

  std::unordered_map<
      std::string,
      std::function<void(const std::string &, const std::string &, ParamSet *)>>
      conversor_;

  void include(const std::string &) const;
  std::string filename_;
};

#endif // !PARSER_HPP
