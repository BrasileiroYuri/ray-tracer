#ifndef API_HPP
#define API_HPP

#include "background.hpp"
#include "film.hpp"
#include "math.hpp"
#include "param_set.hpp"
#include <string>

class App {
public:
  struct RunningOptions {
    bool quick = false;
    std::string filename = "";
    point2 tl = {0, 0}, br = {0, 0};
  };

  static void render();
  static void backGround(const ParamSet &);
  static void film(const ParamSet &);

  static RunningOptions runningOpts_;

private:
  static void write_image();
  static Film film_;
  static BackGroundColor background_;
  static std::string filename_;
  static bool ppm_;
};

#endif // !API_HPP
