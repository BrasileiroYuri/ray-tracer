#include "app.hpp"
#include "ambient_light.hpp"
#include "background.hpp"
#include "blinnphong_integrator.hpp"
#include "blinnphong_material.hpp"
#include "cube.hpp"
#include "direcional_light.hpp"
#include "flat_material.hpp"
#include "geometric_primitive.hpp"
#include "light.hpp"
#include "material.hpp"
#include "math.hpp"
#include "param_set.hpp"
#include "plane.hpp"
#include "point_light.hpp"
#include "prim_list.hpp"
#include "raycast_integrator.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "spot_light.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility> //

// Acessando as opções do main.cpp
namespace Global {
extern std::string outfile;
extern bool quick;
extern int crop[4];
extern bool has_crop;
} // namespace Global

std::unique_ptr<Integrator> App::integrator_;

struct GeneralConfig {
  std::string integratorType;
  std::string filename_ = "";
  std::size_t depth = 0;
  bool ppm_ = true;
};

struct SceneConfig {
  std::unique_ptr<AggregatePrimitive> aggrPrim = std::make_unique<PrimList>();
  std::array<RGBColor, 4> arr;
  std::vector<std::shared_ptr<Light>> lights;
};

GeneralConfig generalConfig;
CameraConfig cameraConfig;
SceneConfig sceneConfig;

std::unordered_map<std::string, std::shared_ptr<Material>> materials;
std::shared_ptr<Material> currMaterial = nullptr;

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

  if (type == "flat") {
    materials[name] = std::make_shared<FlatMaterial>(color);
    std::cout << ">>> Criando material '" << type
              << "' com cor: " << color.str() << ".\n";
  }

  if (type == "blinn") {
    // Extrai os coeficientes de cor (que o parser já converteu para float 0-1)
    auto ka = ps.retrieve<RGBColor>("ambient", {0.1f, 0.1f, 0.1f});
    auto kd = ps.retrieve<RGBColor>("diffuse", {0.5f, 0.5f, 0.5f});
    auto ks = ps.retrieve<RGBColor>("specular", {0.5f, 0.5f, 0.5f});

    // Extrai o expoente de brilho (glossiness)
    auto gloss = ps.retrieve<float>("glossiness", 10.0f);
    auto mirror = ps.retrieve<RGBColor>("mirror");

    std::cout << ">>> Criando material Blinn: '" << name << "'\n";
    materials[name] =
        std::make_shared<BlinnPhongMaterial>(ka, kd, ks, gloss, mirror);
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
  currMaterial = materials[name];
}

void App::material(const ParamSet &ps) {
  auto type = ps.retrieve<std::string>("type");

  if (type.empty()) {
    std::cout << ">>> Material de tipo inválido, usando 'FlatMaterial\n'";
    auto color = ps.retrieve<RGBColor>("color", {0, 0, 0});
    currMaterial = std::make_shared<FlatMaterial>(color);
    return;
  }

  if (type == "flat") {
    auto color = ps.retrieve<RGBColor>("color", {0, 0, 0});
    currMaterial = std::make_shared<FlatMaterial>(color);
  }

  if (type == "blinn") {
    auto ka = ps.retrieve<RGBColor>("ambient", {0.1f, 0.1f, 0.1f});
    auto kd = ps.retrieve<RGBColor>("diffuse", {0.5f, 0.5f, 0.5f});
    auto ks = ps.retrieve<RGBColor>("specular", {0.5f, 0.5f, 0.5f});
    auto gloss = ps.retrieve<float>("glossiness", 10.0f);

    currMaterial = std::make_shared<BlinnPhongMaterial>(ka, kd, ks, gloss);
  }
}

void App::backGround(const ParamSet &ps) {
  std::string key = ps.retrieve<std::string>("type");
  if (key == "colors" || key == "4_colors") {
    sceneConfig.arr[0] = ps.retrieve<RGBColor>("bl");
    sceneConfig.arr[1] = ps.retrieve<RGBColor>("tl");
    sceneConfig.arr[2] = ps.retrieve<RGBColor>("tr");
    sceneConfig.arr[3] = ps.retrieve<RGBColor>("br");

    std::cout << ">>> Usando 'colors': \n";
    std::cout << "* BL: " << sceneConfig.arr[0].str() << "\n";
    std::cout << "* TL: " << sceneConfig.arr[1].str() << "\n";
    std::cout << "* TR: " << sceneConfig.arr[2].str() << "\n";
    std::cout << "* BR: " << sceneConfig.arr[3].str() << "\n";

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
  generalConfig.depth = (std::size_t)ps.retrieve<int>("depth");
}

void App::object(const ParamSet &ps) {
  std::string type = ps.retrieve<std::string>("type");

  if (type == "sphere") {
    sphere(ps);
  } else if (type == "plane") {
    plane(ps);
  } else if (type == "cube") {
    cube(ps);
  } else {
    std::cout << "Objeto " << (type.empty() ? "vazio" : type) << "inválido.\n";
  }
}

// Implementação do método que cria a esfera a partir do XML
void App::sphere(const ParamSet &ps) {
  point3 center = ps.retrieve<point3>("center", {0, 0, 0});
  float radius = ps.retrieve<float>("radius", 1.0f);

  std::cout << ">>> Criando 'Sphere' com raio " << radius << " e centro "
            << center.str() << ".\n";

  float z_min = ps.retrieve<float>("z_min", -radius);
  float z_max = ps.retrieve<float>("z_max", radius);
  float phi_max = ps.retrieve<float>("phi_max", 360.0f);

  /* Instanciando um 'Shape' de Owner único (unique_ptr). */
  auto shape = std::make_unique<Sphere>(center, radius, z_min, z_max, phi_max);

  /* Instanciando um 'Material' de Owner compartilhado (shared_ptr). */
  std::shared_ptr<Material> mat = currMaterial;

  auto geoPrim = std::make_shared<GeometricPrimitive>(std::move(shape), mat);

  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::light_source(const ParamSet &ps) {
  auto type = ps.retrieve<std::string>("type");
  auto intensity = ps.retrieve<RGBColor>("I", {1, 1, 1});
  auto scale = ps.retrieve<RGBColor>("scale", {1, 1, 1});

  if (type == "ambient") {
    sceneConfig.lights.push_back(
        std::make_shared<AmbientLight>(intensity, scale));
  } else if (type == "directional") {
    auto from = ps.retrieve<point3>("from", {0, 1, 0});
    auto to = ps.retrieve<point3>("to", {0, 0, 0});
    sceneConfig.lights.push_back(
        std::make_shared<DirectionalLight>(intensity, scale, from, to));
  } else if (type == "point") {
    auto from = ps.retrieve<point3>("from", {0, 0, 0});
    sceneConfig.lights.push_back(
        std::make_shared<PointLight>(intensity, scale, from));
  } else if (type == "spot") {
    auto from = ps.retrieve<point3>("from", {0, 0, 0});
    auto to = ps.retrieve<point3>("to", {0, 0, 0});
    auto c = ps.retrieve<int>("cutoff", 50);
    auto f = ps.retrieve<int>("falloff", 20);

    sceneConfig.lights.push_back(
        std::make_shared<SpotLight>(intensity, scale, from, to, c, f));
  }
}

void App::integratorConfig(const std::string &type) {
  if (type == "flat") {
    std::cout << ">>> Usando 'RayCastIntegrator'.\n";
    integrator_ = std::make_unique<RayCastIntegrator>();
  } else if (type == "blinn" || type == "blinn_phong") {
    std::cout << ">>> Usando 'BlinnPhongIntegrator'.\n";
    integrator_ = std::make_unique<BlinnPhongIntegrator>();
  } else {
    std::cerr << ">>> Tipo do Integrator não identificado. Usando "
                 "'RayCastIntegrator'.\n";
    integrator_ = std::make_unique<RayCastIntegrator>();
  }

  integrator_->makeCamera(cameraConfig);
}

void App::plane(const ParamSet &ps) {
  point3 point = ps.retrieve<point3>("point", {0, 0, 0});
  vec3 normal = ps.retrieve<vec3>("normal", {0, 1, 0});
  auto shape = std::make_unique<Plane>(point, normal);
  auto geoPrim =
      std::make_shared<GeometricPrimitive>(std::move(shape), currMaterial);
  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::cube(const ParamSet &ps) {
  auto p1 = ps.retrieve<point3>("p1");
  auto p2 = ps.retrieve<point3>("p2");
  auto shape = std::make_unique<Cube>(p1, p2);
  auto geoPrim =
      std::make_shared<GeometricPrimitive>(std::move(shape), currMaterial);
  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::render() {

  integratorConfig(generalConfig.integratorType);
  //  garante que as luzes cheguem ao integrador
  Scene sc(sceneConfig.arr, std::move(sceneConfig.aggrPrim),
           sceneConfig.lights);
  integrator_->render(sc, generalConfig.depth);
  integrator_->write_image(generalConfig.filename_, generalConfig.ppm_);
  sceneConfig.aggrPrim = std::make_unique<PrimList>();
  sceneConfig.lights
      .clear(); // evita que luzes de um render acumulem no próximo
}
