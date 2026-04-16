#include "app.hpp"
#include "background.hpp"
#include "camera.hpp"
#include "film.hpp"
#include "lodepng.h"
#include "math.hpp"
#include "orthographic_camera.hpp"
#include "param_set.hpp"
#include "perspective_camera.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp" 
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <limits> 

// Acessando as opções do main.cpp
namespace Global {
extern std::string outfile;
extern bool quick;
extern int crop[4];
extern bool has_crop;
} 

std::string App::filename_ = "";
bool App::ppm_ = true;

BackGroundColor App::background_ = BackGroundColor();

struct CameraConfig {
  point3 look_from, look_at, up;
};

int fovy = 0;
float aspec = 0.f;
bool has_aspect = false;
CameraConfig config_;
ScreenWindow screen_window_ = ScreenWindow();
std::unique_ptr<Camera> App::camera_ = nullptr;

// Inicialização da lista estática de objetos da cena
std::vector<std::unique_ptr<Primitive>> App::primitives_;

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
  int x = ps.retrieve<int>("w_res");
  int y = ps.retrieve<int>("h_res");

  filename_ = Global::outfile.empty() ? xml_file : Global::outfile;
  if (Global::quick) {
    x /= 4;
    y /= 4;
  }

  ppm_ = ps.retrieve<std::string>("img_type") == "ppm";
  camera_->film_ = Film((std::size_t)x, (std::size_t)y);
}

void App::camera(const ParamSet &ps) {
  auto type = ps.retrieve<std::string>("type");
  if (type == "perspective")
    camera_ = std::make_unique<Perspective>();
  else
    camera_ = std::make_unique<Orthographic>();
  
  fovy = ps.retrieve<int>("fovy");
  aspec = ps.retrieve<float>("frame_aspect_ratio");

  if (ps.has_elem("screen_window"))
    screen_window_ = ps.retrieve<ScreenWindow>("screen_window");
}

void App::lookat(const ParamSet &ps) {
  config_.look_from = ps.retrieve<point3>("look_from", {0, 0, 0});
  config_.look_at = ps.retrieve<point3>("look_at", {0, 0, 0});
  config_.up = ps.retrieve<point3>("up", {0, 0, 0});
}

void App::addIntegrator(ParamSet ps) {
    // Apenas recupera o tipo para validar a leitura
    std::string type = ps.retrieve<std::string>("type");
}
/*
O integrator acima ainda não tem implementação, mas é necessário para validar a leitura do XML 
e evitar erros de elemento desconhecido. Fica pra você fazer no projeto 4 ;)
*/ 
void App::addObject(ParamSet ps) {
    std::string type = ps.retrieve<std::string>("type");
    
    if (type == "sphere") {
        addSphere(ps);
    } 
    else if (type == "plane") {
        addPlane(ps);
    } 
    else if (type == "triangle") {
        addTriangle(ps);
    }
}

// Implementação do método que cria a esfera a partir do XML
void App::addSphere(ParamSet ps) {
    point3 center = ps.retrieve<point3>("center", {0, 0, 0});
    float radius = ps.retrieve<float>("radius", 1.0f);

    float z_min = -radius;
    float z_max = radius;
    float phi_max = 360.0f;

    if (ps.has_elem("z_min")) z_min = ps.retrieve<float>("z_min");
    if (ps.has_elem("z_max")) z_max = ps.retrieve<float>("z_max");
    if (ps.has_elem("phi_max")) phi_max = ps.retrieve<float>("phi_max");

    std::cout << "Sphere XML -> z_min: " << z_min << " z_max: " << z_max << " phi: " << phi_max << std::endl;

    primitives_.push_back(std::make_unique<Sphere>(center, radius, z_min, z_max, phi_max)); 
}

// implementação do método que cria o plano a partir do XML
void App::addPlane(ParamSet ps) {
    point3 p = ps.retrieve<point3>("p", {0, 0, 0});
    vec3 u = ps.retrieve<vec3>("u", {1, 0, 0});
    vec3 v = ps.retrieve<vec3>("v", {0, 1, 0});

    primitives_.push_back(std::make_unique<Plane>(p, u, v));
}

// implementação do método que cria o triângulo a partir do XML
void App::addTriangle(ParamSet ps) {
   
    point3 v0 = ps.retrieve<point3>("v0", {-1, 0, 0});
    point3 v1 = ps.retrieve<point3>("v1", {1, 0, 0});
    point3 v2 = ps.retrieve<point3>("v2", {0, 1, 0});

    primitives_.push_back(std::make_unique<Triangle>(v0, v1, v2));
    std::cout << "Triangle add in scene" << std::endl;
}

void App::calculateScreenWindow() {
  float aspectratio =
      aspec ? aspec : (float)camera_->film_.width() / camera_->film_.height();

  if (fovy) {
    float h = std::tan((fovy * M_PI / 180.0f) / 2.0f); // graus para radianos
    screen_window_.l_ = -aspectratio * h;
    screen_window_.r_ = aspectratio * h;
    screen_window_.b_ = -h;
    screen_window_.t_ = h;
  }
}

void App::render() {
  
  std::cout << "Quantidade de objetos na cena: " << primitives_.size() << std::endl;
  
  std::size_t h = camera_->film_.height(), w = camera_->film_.width();
  camera_->getFrame(config_.look_from, config_.look_at, config_.up);
  
  // Sempre calcula se houver valores definidos
if (fovy > 0 || aspec > 0) {
    calculateScreenWindow();
}

camera_->window(screen_window_.l_, screen_window_.r_, screen_window_.b_,
                  screen_window_.t_);

  int x0 = 0, x1 = w, y0 = 0, y1 = h;
  if (Global::has_crop) {
    x0 = std::max(0, Global::crop[0]);
    x1 = std::min((int)w, Global::crop[1]);
    y0 = std::max(0, Global::crop[2]);
    y1 = std::min((int)h, Global::crop[3]);
  }

  for (int i = y0; i < y1; i++) {
    for (int j = x0; j < x1; j++) {
      // Gera o raio com limites t_min e t_max
      Ray r = camera_->generateRay(j, i);
      
      bool hit_anything = false;
      float t_hit = 0;

      // Loop de Interseção: Testa contra todos os objetos
      for (const auto& primitive : primitives_) {
        float t_current;
        if (primitive->intersect(r, t_current)) {
          // Se atingiu, encurtamos o raio para buscar apenas o que estiver à frente
          r.set_t_max(t_current);
          hit_anything = true;
        }
      }

      if (hit_anything) {
        // Pinta de uma cor sólida para testar a visibilidade
        camera_->film_.add(j, i, RGBColor(255, 0, 0)); 
      } else {
        // Se falhou todos os testes, usa o fundo
        float u = float(j) / float(w - 1);
        float v = 1.f - (float(i) / (float)(h - 1));
        camera_->film_.add(j, i, background_.sample(u, v));
      }
    }
  }

  write_image();
}

void App::write_image() {
  if (!ppm_) {
    lodepng::encode(filename_, camera_->film_.data(),
                    (unsigned)camera_->film_.width(),
                    (unsigned)camera_->film_.height());
  } else {
    FILE *f = fopen(filename_.c_str(), "w");
    if(!f) return;
    fprintf(f, "P3\n%d %d\n255\n", (int)camera_->film_.width(),
            (int)camera_->film_.height());
    for (size_t i = 0; i < camera_->film_.data().size(); i += 4)
      fprintf(f, "%d %d %d\n", camera_->film_.data()[i],
              camera_->film_.data()[i + 1], camera_->film_.data()[i + 2]);
    fclose(f);
  }
}