#include "app.hpp"
#include "background.hpp"
#include "film.hpp"
#include "lodepng.h"
#include <algorithm>
#include <iostream>

// Acessando as opções do main.cpp
namespace Global {
    extern std::string outfile;
    extern bool quick;
    extern int crop[4];
    extern bool has_crop;
}

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
    background_ = BackGroundColor(arr);
}

void App::film(const ParamSet &ps) {
    std::string xml_file = ps.retrieve<std::string>("filename");
    int x = ps.retrieve<int>("x_res");
    int y = ps.retrieve<int>("y_res");

    // Prioridade CLI > XML
    filename_ = Global::outfile.empty() ? xml_file : Global::outfile;
    if (Global::quick) { x /= 4; y /= 4; }

    ppm_ = ps.retrieve<std::string>("img_type") == "ppm";
    film_ = Film((std::size_t)x, (std::size_t)y);
}

void App::render() {
    std::size_t h = film_.height(), w = film_.width();
    
    // Configuração do Crop Window
    int x0 = 0, x1 = w, y0 = 0, y1 = h;
    if (Global::has_crop) {
        x0 = std::max(0, Global::crop[0]);
        x1 = std::min((int)w, Global::crop[1]);
        y0 = std::max(0, Global::crop[2]);
        y1 = std::min((int)h, Global::crop[3]);
    }

    for (int i = y0; i < y1; i++) {
        for (int j = x0; j < x1; j++) {
            float u = float(j) / float(w - 1);
            float v = 1.f - (float(i) / (float)(h - 1));
            film_.add(j, i, background_.sample(u, v));
        }
    }
    write_image();
}

void App::write_image() {
    if (!ppm_) {
        lodepng::encode(filename_, film_.data(), (unsigned)film_.width(), (unsigned)film_.height());
    } else {
        FILE *f = fopen(filename_.c_str(), "w");
        fprintf(f, "P3\n%d %d\n255\n", (int)film_.width(), (int)film_.height());
        for (size_t i = 0; i < film_.data().size(); i += 4)
            fprintf(f, "%d %d %d\n", film_.data()[i], film_.data()[i+1], film_.data()[i+2]);
        fclose(f);
    }
}