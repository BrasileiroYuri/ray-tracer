#ifndef FILM_HPP
#define FILM_HPP

#include "background.hpp"
#include <cstddef>
#include <vector>
#include <algorithm> // std::clamp

class Film {
public:
  Film(std::size_t width, std::size_t height)
      : width_(width), height_(height), data_(width * height * 4) {}

  void add(std::size_t i, std::size_t j, RGBColor color) {
    if (j >= height_ || i >= width_)
      return;

    auto base = (j * width_ + i) * 4;

    // Conversão de float [0.0, 1.0+] para unsigned char [0, 255]
    // O std::clamp impede que brilhos especulares > 1.0 causem estouro de bits
    data_[base + 0] = static_cast<unsigned char>(std::clamp(color.r, 0.0f, 1.0f) * 255.0f);
    data_[base + 1] = static_cast<unsigned char>(std::clamp(color.g, 0.0f, 1.0f) * 255.0f);
    data_[base + 2] = static_cast<unsigned char>(std::clamp(color.b, 0.0f, 1.0f) * 255.0f);
    data_[base + 3] = static_cast<unsigned char>(std::clamp(color.a, 0.0f, 1.0f) * 255.0f);
  }

  const std::vector<unsigned char> &data() const { return data_; }
  std::size_t height() const { return height_; }
  std::size_t width() const { return width_; }

private:
  std::size_t width_;
  std::size_t height_;
  std::vector<unsigned char> data_;
};

#endif