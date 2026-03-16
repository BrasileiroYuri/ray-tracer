#ifndef FILM_HPP
#define FILM_HPP

#include "background.hpp"
#include <cstddef>
#include <vector>

class Film {
public:
  Film(std::size_t width, std::size_t height)
      : width_(width), height_(height), data_(width * height * 4) {}

  void add(std::size_t, std::size_t, RGBColor);
  std::size_t width() const;
  std::size_t height() const;

  void write_image() const;

private:
  std::size_t width_;
  std::size_t height_;
  std::vector<unsigned char> data_;
};

#endif // !FILM_HPP
