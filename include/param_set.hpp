#ifndef PARAM_SET_HPP
#define PARAM_SET_HPP

#include <memory>
#include <string>
#include <unordered_map>

class ParamSet {
public:
  bool has_elem(const std::string &key) const {
    return map_.find(key) != map_.end();
  }

  template <typename T>
  T retrieve(const std::string &key, const T &def_value = T{}) const {
    auto it = map_.find(key);

    if (it == map_.end())
      return def_value;

    auto *typed = static_cast<ValueType<T> *>(it->second.get());
    return typed->value_;
  }

  template <typename T> void add(const std::string &key, const T &value) {
    map_[key] = std::make_unique<ValueType<T>>(value);
  }

private:
  struct GenericType {
    virtual ~GenericType() {};
  };

  template <typename T> struct ValueType : public GenericType {
    T value_;
    ValueType(const T &value) : value_(value) {};
  };

  std::unordered_map<std::string, std::unique_ptr<GenericType>> map_;
};

#endif // !PARAM_SET_HPP
