#include "film.hpp"

void Film::add(std::size_t i, std::size_t j, RGBColor r) {
  auto base = i * width_ * 4 + j * 4;

  data_[base + 0] = r.R;
  data_[base + 1] = r.G;
  data_[base + 2] = r.B;
}

void Film::write_image() const {}

std::size_t Film::height() const { return height_; }
std::size_t Film::width() const { return width_; }
