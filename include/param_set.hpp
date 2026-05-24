#ifndef PARAM_SET_HPP
#define PARAM_SET_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ParamSet {
public:
  bool has_elem(const std::string &key) const {
    return map_.find(key) != map_.end();
  }

  /*
   * Método padrão, feito para retornar apenas uma entidade do tipo T.
   * @note: programa quebra quando essa função é chamada onde deveria ser usado
   * get (parser de lista e retrieve de lista).
   */
  template <typename T>
  T retrieve(const std::string &key, const T &def_value = T{}) const {
    auto it = map_.find(key);

    if (it == map_.end())
      return def_value;

    /// Aqui basicamente fazemos o cast do elemento 0 do vetor.
    auto *typed = static_cast<ValueType<T> *>(it->second.at(0).get());
    return typed->value_;
  }

  /*
   * Método para parser de listas de argumentos, por exemplo, vértices.
   */
  template <typename T>
  std::vector<T> get(const std::string &key, const T &def_value = T{}) const {
    auto it = map_.find(key);

    if (it == map_.end())
      return {def_value};

    std::vector<T> vec;

    /*
     * Parte chave.
     * Nosso valor do dicionário é apenas um vector para
     * unique_ptr<GenericType>. Dessa forma, é necessário para cada unique_ptr
     * fazer o cast para 'ValueType<T> *' para futuramente pegar value_.
     *
     * Poderia ser:
     * auto *typed = static_cast<ValueType<T> *>(elem.get());
     * vec.push_back(typed->value_);
     */
    for (auto &elem : it->second)
      vec.push_back(static_cast<ValueType<T> *>(elem.get())->value_);

    return vec;
  }

  template <typename T> void add(const std::string &key, const T &value) {
    map_[key].push_back(std::make_unique<ValueType<T>>(value));
  }

private:
  struct GenericType {
    virtual ~GenericType() {};
  };

  template <typename T> struct ValueType : public GenericType {
    T value_;
    ValueType(const T &value) : value_(value) {};
  };

  std::unordered_map<std::string, std::vector<std::unique_ptr<GenericType>>>
      map_;
};

#endif // !PARAM_SET_HPP
