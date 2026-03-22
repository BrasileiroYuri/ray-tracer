#include "app.hpp"
#include "background.hpp"
#include "film.hpp"
#include "lodepng.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

std::string App::filename_ = "";
bool App::ppm_ = true;
Film App::film_ = Film();
BackGroundColor App::background_ = BackGroundColor();

void App::backGround(const ParamSet &ps) {
  std::array<RGBColor, 4> arr;
  if (ps.retrieve<std::string>("type") == "colors") {
    arr[0] = ps.retrieve<RGBColor>("bl");
    arr[1] = ps.retrieve<RGBColor>("tl");
    arr[2] = ps.retrieve<RGBColor>("tr");
    arr[3] = ps.retrieve<RGBColor>("br");
  } else {
    RGBColor color = ps.retrieve<RGBColor>("color");
    std::fill(arr.begin(), arr.end(), color);
  }

  BackGroundColor b(arr);
  background_ = b;
}

void App::film(const ParamSet &ps) {

  filename_ = ps.retrieve<std::string>("filename");
  int x = ps.retrieve<int>("x_res");
  int y = ps.retrieve<int>("y_res");
  ppm_ = ps.retrieve<std::string>("img_type") == "ppm" ? true : false;
  Film f((std::size_t)x, (std::size_t)y);

  film_ = f;
}

void App::write_image() {
  std::cout << (ppm_ ? "1\n" : "0\n");
  if (!ppm_) {
    std::vector<unsigned char> png;
    lodepng::encode(png, film_.data().data(), (unsigned)film_.width(),
                    (unsigned)film_.height(), LCT_RGBA, 8);

    lodepng::save_file(png, filename_);
    return;
  }

  FILE *f = fopen(filename_.c_str(), "w");
  fprintf(f, "P3\n%d %d\n255\n", (int)film_.width(), (int)film_.height());

  std::size_t max = film_.width() * film_.height() * 4;

  for (std::size_t i = 0; i < max; i += 4)
    fprintf(f, "%d %d %d\n", (int)film_.data()[i], (int)film_.data()[i + 1],
            (int)film_.data()[i + 2]);
}

void App::render() {

  std::size_t h = film_.height(), w = film_.width();
  for (std::size_t i = 0; i < h; i++) {
    for (std::size_t j = 0; j < w; j++) {
      RGBColor color = background_.sample(float(j) / float(w - 1),
                                          1.f - (float(i) / (float)(h - 1)));
      film_.add(i, j, color);
    }
  }

  write_image();
}
