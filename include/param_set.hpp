#ifndef PARAM_SET_HPP
#define PARAM_SET_HPP

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

    auto val = dynamic_cast<ValueType<T> *>(it->second);
    if (!val)
      return def_value;

    return val->value_;
  }

  template <typename T> void add(const std::string &key, const T &value) {
    if (map_.count(key)) {
      delete map_[key];
    }
    map_[key] = new ValueType<T>(value);
  }
  void remove() {
    for (auto &e : map_) {
      delete e.second;
    }
    map_.clear();
  }

private:
  struct GenericType {
    virtual ~GenericType() {};
  };

  template <typename T> struct ValueType : public GenericType {
    T value_;
    ValueType(const T &value) : value_(value) {};
  };

  std::unordered_map<std::string, GenericType *> map_;
};

#endif // !PARAM_SET_HPP
