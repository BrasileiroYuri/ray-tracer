#ifndef API_HPP
#define API_HPP

#include "background.hpp"
#include "film.hpp"
#include "param_set.hpp"

class Api {
public:
  static void render();

  static void backGround(const ParamSet &);
  static void film(const ParamSet &);

private:
  static void run();

  static Film film_;
  static BackGroundColor background_;
};

#endif // !API_HPP
