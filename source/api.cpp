#include "api.hpp"
#include <background.hpp>
#include <iostream>

// Inicialização dos membros estáticos com valores padrão
Film Api::film_(800, 600, "output.png");
BackGroundColor Api::background_({RGBColor(0,0,0), RGBColor(0,0,0), RGBColor(0,0,0), RGBColor(0,0,0)});

void Api::film(const ParamSet &ps) {
    int x_res = ps.retrieve<int>("x_res", 800);
    int y_res = ps.retrieve<int>("y_res", 600);
    std::string filename = ps.retrieve<std::string>("filename", "output.png");
    
    film_ = Film(x_res, y_res, filename);
}

void Api::backGround(const ParamSet &ps) {
    std::string type = ps.retrieve<std::string>("type", "single_color");
    
    if (type == "single_color") {
        RGBColor color = ps.retrieve<RGBColor>("color", RGBColor(0,0,0));
        background_ = BackGroundColor({color, color, color, color});
    } else if (type == "4_colors") {
        RGBColor bl = ps.retrieve<RGBColor>("bl", RGBColor(0,0,0));
        RGBColor tl = ps.retrieve<RGBColor>("tl", RGBColor(0,0,0));
        RGBColor tr = ps.retrieve<RGBColor>("tr", RGBColor(0,0,0));
        RGBColor br = ps.retrieve<RGBColor>("br", RGBColor(0,0,0));
        background_ = BackGroundColor({bl, tl, tr, br});
    }
}

void Api::render() {
    auto w = film_.width();
    auto h = film_.height();

    std::cout << "Renderizando cena (" << w << "x" << h << ")..." << std::endl;

    for (int j = 0; j < (int)h; j++) {
        for (int i = 0; i < (int)w; i++) {
            // Normalizei i e j para o intervalo [0, 1] para a interpolação
            float u = float(i) / float(w);
            float v = float(j) / float(h);
            
            RGBColor color = background_.sample(u, v);
            film_.add(i, j, color);
        }
    }

    film_.write_image();
}