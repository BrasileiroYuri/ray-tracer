#ifndef FILM_HPP
#define FILM_HPP

#include "background.hpp"
#include <cstddef>
#include <vector>

class Film {
public:
  Film(std::size_t width, std::size_t height)
      : width_(width), height_(height), data_(width * height * 4) {}

  //!< i - column, j - row
  void add(std::size_t i, std::size_t j, RGBColor r) {
    if (j >= height_ || i >= width_)
      return;

    auto base = j * width_ * 4 + i * 4;

    data_[base + 0] = r.R;
    data_[base + 1] = r.G;
    data_[base + 2] = r.B;
    data_[base + 3] = r.A;
  }

  const std::vector<unsigned char> &data() const { return data_; }
  std::size_t height() const { return height_; }
  std::size_t width() const { return width_; }

private:
  std::size_t width_;
  std::size_t height_;
  std::vector<unsigned char> data_;
};

#endif // !FILM_HPP
