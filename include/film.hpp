#ifndef FILM_HPP
#define FILM_HPP

#include "background.hpp"
#include <algorithm> // std::clamp
#include <cmath>     // std::pow
#include <cstddef>
#include <vector>

class Film {
public:
  Film(std::size_t width, std::size_t height, bool gamma_corrected = true)
      : width_(width), height_(height), data_(width * height * 4),
        gamma_corrected_(gamma_corrected) {}

  void add(std::size_t i, std::size_t j, RGBColor color) {
    if (j >= height_ || i >= width_)
      return;

    auto base = (j * width_ + i) * 4;

    // Sem isso, valores lineares parecem muito escuros no monitor.
    auto encode = [&](float v) -> unsigned char {
      float clamped = std::clamp(v, 0.0f, 1.0f);
      if (gamma_corrected_)
        clamped = std::pow(clamped, 1.0f / 2.2f);

      return static_cast<unsigned char>(clamped * 255.0f);
    };

    data_[base + 0] = encode(color.r_);
    data_[base + 1] = encode(color.g_);
    data_[base + 2] = encode(color.b_);
    data_[base + 3] = encode(color.a_);
  }

  const std::vector<unsigned char> &data() const { return data_; }
  std::size_t height() const { return height_; }
  std::size_t width() const { return width_; }

private:
  std::size_t width_;
  std::size_t height_;
  std::vector<unsigned char> data_;
  bool gamma_corrected_;
};

#endif
