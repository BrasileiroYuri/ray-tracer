#ifndef PARAM_SET_HPP
#define PARAM_SET_HPP

#include <string>
#include <unordered_map>

class ParamSet {
public:
  template <typename T> T retrieve(const std::string &, const T &) const;
  template <typename T> void add(const std::string &, const T &);

private:
  struct GenericType {
    virtual ~GenericType() {};
  };

  template <typename T> struct ValueType : public GenericType {
    T value_;
    ValueType(const T &value) : value_(value) {};
  };

  std::unordered_map<std::string, GenericType> map_;
};

#endif // !PARAM_SET_HPP
