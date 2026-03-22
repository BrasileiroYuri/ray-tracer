#ifndef FILM_HPP
#define FILM_HPP

#include "background.hpp"
#include <cstddef>
#include <vector>
#include <string>

class Film {
public:
    Film(std::size_t width, std::size_t height, std::string filename = "output.png")
        : width_(width), height_(height), filename_(filename), data_(width * height * 4) {}

    void add(std::size_t i, std::size_t j, RGBColor r);
    std::size_t width() const;
    std::size_t height() const;
    void write_image() const;

private:
    std::size_t width_;
    std::size_t height_;
    std::string filename_;
    std::vector<unsigned char> data_;
};

#endif