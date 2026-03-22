#include "film.hpp"
#include "lodepng.h"
#include <iostream>

void Film::add(std::size_t i, std::size_t j, RGBColor r) {
    // Inverti o eixo J para que (0,0) seja o canto inferior esquerdo, conforme padrão de Ray Tracing
    auto row = (height_ - 1 - j); 
    auto base = (row * width_ + i) * 4;

    if (base + 3 < data_.size()) {
        data_[base + 0] = r.R;
        data_[base + 1] = r.G;
        data_[base + 2] = r.B;
        data_[base + 3] = 255; // Alpha opaco
    }
}

void Film::write_image() const {
    unsigned error = lodepng::encode(filename_, data_, width_, height_);
    if (error) {
        std::cerr << "Erro lodepng " << error << ": " << lodepng_error_text(error) << std::endl;
    } else {
        std::cout << "Arquivo gerado com sucesso: " << filename_ << std::endl;
    }
}

std::size_t Film::height() const { return height_; }
std::size_t Film::width() const { return width_; }