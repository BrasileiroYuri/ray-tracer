#include "app.hpp"
#include "background.hpp"
#include "flat_material.hpp"
#include "material.hpp"
#include "math.hpp"
#include "param_set.hpp"
#include "prim_list.hpp"
#include "primitive.hpp"
#include "raycast_integrator.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include <array>
#include <iostream>
#include <memory>
#include <string>

// Acessando as opções do main.cpp
namespace Global {
extern std::string outfile;
extern bool quick;
extern int crop[4];
extern bool has_crop;
} // namespace Global

std::unique_ptr<AggregatePrimitive> App::aggrPrim =
    std::make_unique<PrimList>();

std::unique_ptr<Integrator> App::integrator_;
std::unordered_map<std::string, std::shared_ptr<Material>> materials;
std::shared_ptr<Material> actualMaterial =
    nullptr; /* Apenas observa um material, não detém o Ownership. */

struct GeneralConfig {
  std::string integratorType;
  std::string filename_ = "";
  bool ppm_ = true;
};

struct SceneConfig {
  std::array<RGBColor, 4> arr;
};

GeneralConfig generalConfig;
CameraConfig cameraConfig;
SceneConfig sceneConfig;

void App::make_named_material(const ParamSet &ps) {
  auto name = ps.retrieve<std::string>("name");

  /* Se o material nomeado não tem nome, não salvamos. */
  if (name.empty()) {
    std::cerr << ">>> Material nomeado sem nome! Não será salvo.\n";
    return;
  }

  /* Usamos como base o FlatMaterial */
  auto type = ps.retrieve<std::string>("type", "flat");
  auto color = ps.retrieve<RGBColor>("color", {0, 0, 0});

  std::cout << ">>> Criando material '" << type << "' com cor: " << color.str()
            << ".\n";

  if (type == "flat") {
    materials[name] = std::make_shared<FlatMaterial>(color);
  }
}

void App::named_material(const ParamSet &ps) {
  auto name = ps.retrieve<std::string>("name");

  auto it = materials.find(name);
  if (it == materials.end()) {
    std::cerr << ">>> Material não encontrado! Continuando com o material "
                 "anterior.\n";
    return;
  }

  std::cout << ">>> Usando material '" << name << "'.\n";
  actualMaterial = materials[name];
}

void App::material(const ParamSet &ps) {
  auto type = ps.retrieve<std::string>("type");

  if (type.empty()) {
    std::cout << ">>> Material de tipo inválido, usando 'FlatMaterial\n'";
    auto color = ps.retrieve<RGBColor>("color", {0, 0, 0});
    actualMaterial = std::make_shared<FlatMaterial>(color);
    return;
  }

  if (type == "flat") {
    auto color = ps.retrieve<RGBColor>("color", {0, 0, 0});
    actualMaterial = std::make_shared<FlatMaterial>(color);
  }
}

void App::backGround(const ParamSet &ps) {
  std::string key = ps.retrieve<std::string>("type");
  if (key == "colors") {
    sceneConfig.arr[0] = ps.retrieve<RGBColor>("bl");
    sceneConfig.arr[1] = ps.retrieve<RGBColor>("tl");
    sceneConfig.arr[2] = ps.retrieve<RGBColor>("tr");
    sceneConfig.arr[3] = ps.retrieve<RGBColor>("br");

    std::cout << ">>> Usando 'colors': \n";
    std::cout << "BL: " << sceneConfig.arr[0].str() << "\n";
    std::cout << "TL: " << sceneConfig.arr[1].str() << "\n";
    std::cout << "TR: " << sceneConfig.arr[2].str() << "\n";
    std::cout << "BR: " << sceneConfig.arr[3].str() << "\n";

  } else if (key == "single_color") {
    RGBColor color = ps.retrieve<RGBColor>("color", {0, 0, 0});
    std::fill(sceneConfig.arr.begin(), sceneConfig.arr.end(), color);

    std::cout << ">>> Usando 'single_color': " << color.str() << "\n";
  } else {
    RGBColor color = {0, 0, 0};
    std::fill(sceneConfig.arr.begin(), sceneConfig.arr.end(), color);

    std::cout << ">>> Cor indefinida. Usando: " << color.str() << "\n";
  }
}

void App::film(const ParamSet &ps) {
  std::string xml_file = ps.retrieve<std::string>("filename");

  cameraConfig.w_res = ps.retrieve<int>("w_res");
  cameraConfig.h_res = ps.retrieve<int>("h_res");

  generalConfig.filename_ =
      Global::outfile.empty() ? xml_file : Global::outfile;

  if (Global::quick) {
    cameraConfig.w_res /= 4;
    cameraConfig.h_res /= 4;
  }

  std::cout << ">>> Largura do 'Film': " << cameraConfig.w_res << "\n";
  std::cout << ">>> Altura do 'Film': " << cameraConfig.h_res << "\n";

  generalConfig.ppm_ = ps.retrieve<std::string>("img_type") == "ppm";
}

void App::camera(const ParamSet &ps) {
  cameraConfig.type = ps.retrieve<std::string>("type");

  /* Se houver 'screen_window_', damos preferência. */
  if (ps.has_elem("screen_window")) {
    auto sw = ps.retrieve<ScreenWindow>("screen_window", {0.0, 0.0, 0.0, 0.0});
    cameraConfig.l_ = sw.l_;
    cameraConfig.r_ = sw.r_;
    cameraConfig.b_ = sw.b_;
    cameraConfig.t_ = sw.t_;
    return;
  }

  /* Se não, trabalhamos com 'fovy', 'frame_aspect_ratio' ou w_res/h_res. */
  cameraConfig.fovy = ps.retrieve<int>("fovy", 0);
  cameraConfig.aspec = ps.retrieve<float>("frame_aspect_ratio", 0.0);
}

void App::lookat(const ParamSet &ps) {
  cameraConfig.look_from = ps.retrieve<point3>("look_from", {0, 0, 0});
  cameraConfig.look_at = ps.retrieve<point3>("look_at", {0, 0, 0});
  cameraConfig.up = ps.retrieve<point3>("up", {0, 0, 0});
}

void App::integrator(const ParamSet &ps) {
  generalConfig.integratorType = ps.retrieve<std::string>("type");
}

void App::object(const ParamSet &ps) {
  std::string type = ps.retrieve<std::string>("type");

  if (type == "sphere") {
    sphere(ps);
  }
}

// Implementação do método que cria a esfera a partir do XML
void App::sphere(const ParamSet &ps) {
  point3 center = ps.retrieve<point3>("center", {0, 0, 0});
  float radius = ps.retrieve<float>("radius", 1.0f);

  float z_min = ps.retrieve<float>("z_min", -radius);
  float z_max = ps.retrieve<float>("z_max", radius);
  float phi_max = ps.retrieve<float>("phi_max", 360.0f);

  std::unique_ptr<Sphere> sphere =
      std::make_unique<Sphere>(center, radius, z_min, z_max, phi_max);
}

void App::integratorConfig(const std::string &type) {
  if (type == "flat") {
    std::cout << ">>> Usando 'RayCastIntegrator'.\n";
    integrator_ = std::make_unique<RayCastIntegrator>();
  } else {
    std::cerr << ">>> Tipo do Integrator não identificado. Usando "
                 "'RayCastIntegrator'.\n";
    integrator_ = std::make_unique<RayCastIntegrator>();
  }

  integrator_->makeCamera(cameraConfig);
}

void scenConfig() {}

void App::render() {
  integratorConfig(generalConfig.integratorType);
  scenConfig();

  integrator_->render(void);

  /*
  for (int i = y0; i < y1; i++) {
    for (int j = x0; j < x1; j++) {
      // Gera o raio com limites t_min e t_max
      Ray r = camera_->generateRay(j, i);

#ifdef DEBUG
      std::cout << "\nFirst ray: " << r.str();
#endif
      bool hit_anything = false;

      // Loop de Interseção: Testa contra todos os objetos
      for (const auto &primitive : primitives_) {
        float t_current;
        if (primitive->intersect(r, t_current)) {
          // Se atingiu, encurtamos o raio para buscar apenas o que estiver à
          // frente
          r.set_t_max(t_current);
          hit_anything = true;
        }
      }

#ifdef DEBUG
      std::cout << "\nLast ray: " << r.str();
#endif
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
*/
  integrator_->write_image(generalConfig.filename_, generalConfig.ppm_);
}
