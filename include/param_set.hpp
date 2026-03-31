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

    if (map_.find(key) == map_.end())
      return def_value;

    auto val = dynamic_cast<ValueType<T> *>(map_.at(key));
    return val->value_;
  }

  template <typename T> void add(const std::string &key, const T &value) {
    ValueType<T> *vt = new ValueType<T>(value);
    map_[key] = vt;
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
