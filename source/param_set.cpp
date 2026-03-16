#include "param_set.hpp"

template <typename T>
T ParamSet::retrieve(const std::string &key, const T &def_value) const {

  if (map_.find(key) == map_.end())
    return def_value;

  return map_.at(key);
}
template <typename T>
void ParamSet::add(const std::string &key, const T &value) {
  ValueType<T> vt(value);
  map_[key] = vt;
}
