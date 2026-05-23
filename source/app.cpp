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
#include "pyramid.hpp"
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

template <typename T> using vec = std::vector<T>;

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
  auto name = ps.retrieve<vec<std::string>>("name")[0];

  /* Se o material nomeado não tem nome, não salvamos. */
  if (name.empty()) {
    std::cerr << ">>> Material nomeado sem nome! Não será salvo.\n";
    return;
  }

  /* Usamos como base o FlatMaterial */
  auto type = ps.retrieve<vec<std::string>>("type", {"flat"})[0];
  auto color = ps.retrieve<vec<RGBColor>>("color", {{0, 0, 0}})[0];

  if (type == "flat") {
    materials[name] = std::make_shared<FlatMaterial>(color);
    std::cout << ">>> Criando material '" << type
              << "' com cor: " << color.str() << ".\n";
  }

  if (type == "blinn") {
    // Extrai os coeficientes de cor (que o parser já converteu para float 0-1)
    auto ka = ps.retrieve<vec<RGBColor>>("ambient", {{0.1f, 0.1f, 0.1f}})[0];
    auto kd = ps.retrieve<vec<RGBColor>>("diffuse", {{0.5f, 0.5f, 0.5f}})[0];
    auto ks = ps.retrieve<vec<RGBColor>>("specular", {{0.5f, 0.5f, 0.5f}})[0];

    // Extrai o expoente de brilho (glossiness)
    auto gloss = ps.retrieve<vec<float>>("glossiness", {10.0f})[0];
    auto mirror = ps.retrieve<vec<RGBColor>>("mirror", {{0, 0, 0}})[0];

    std::cout << ">>> Criando material Blinn: '" << name << "'\n";
    materials[name] =
        std::make_shared<BlinnPhongMaterial>(ka, kd, ks, gloss, mirror);
  }
}

void App::named_material(const ParamSet &ps) {
  auto name = ps.retrieve<vec<std::string>>("name")[0];

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
  auto type = ps.retrieve<vec<std::string>>("type")[0];

  if (type.empty()) {
    std::cout << ">>> Material de tipo inválido, usando 'FlatMaterial\n'";
    auto color = ps.retrieve<vec<RGBColor>>("color")[0];
    currMaterial = std::make_shared<FlatMaterial>(color);
    return;
  }

  if (type == "flat") {
    auto color = ps.retrieve<vec<RGBColor>>("color")[0];
    currMaterial = std::make_shared<FlatMaterial>(color);
  }

  if (type == "blinn") {
    auto ka = ps.retrieve<vec<RGBColor>>("ambient", {{0.1f, 0.1f, 0.1f}})[0];
    auto kd = ps.retrieve<vec<RGBColor>>("diffuse", {{0.5f, 0.5f, 0.5f}})[0];
    auto ks = ps.retrieve<vec<RGBColor>>("specular", {{0.5f, 0.5f, 0.5f}})[0];
    auto gloss = ps.retrieve<vec<float>>("glossiness", {10.0f})[0];

    currMaterial = std::make_shared<BlinnPhongMaterial>(ka, kd, ks, gloss);
  }
}

void App::backGround(const ParamSet &ps) {
  std::string key = ps.retrieve<vec<std::string>>("type")[0];
  if (key == "colors" || key == "4_colors") {
    sceneConfig.arr[0] = ps.retrieve<vec<RGBColor>>("bl")[0];
    sceneConfig.arr[1] = ps.retrieve<vec<RGBColor>>("tl")[0];
    sceneConfig.arr[2] = ps.retrieve<vec<RGBColor>>("tr")[0];
    sceneConfig.arr[3] = ps.retrieve<vec<RGBColor>>("br")[0];

    std::cout << ">>> Usando 'colors': \n";
    std::cout << "* BL: " << sceneConfig.arr[0].str() << "\n";
    std::cout << "* TL: " << sceneConfig.arr[1].str() << "\n";
    std::cout << "* TR: " << sceneConfig.arr[2].str() << "\n";
    std::cout << "* BR: " << sceneConfig.arr[3].str() << "\n";

  } else if (key == "single_color") {
    RGBColor color = ps.retrieve<vec<RGBColor>>("color")[0];
    std::fill(sceneConfig.arr.begin(), sceneConfig.arr.end(), color);

    std::cout << ">>> Usando 'single_color': " << color.str() << "\n";
  } else {
    RGBColor color = {0, 0, 0};
    std::fill(sceneConfig.arr.begin(), sceneConfig.arr.end(), color);

    std::cout << ">>> Cor indefinida. Usando: " << color.str() << "\n";
  }
}

void App::film(const ParamSet &ps) {
  std::string xml_file = ps.retrieve<vec<std::string>>("filename")[0];

  cameraConfig.w_res = ps.retrieve<vec<int>>("w_res")[0];
  cameraConfig.h_res = ps.retrieve<vec<int>>("h_res")[0];

  generalConfig.filename_ =
      Global::outfile.empty() ? xml_file : Global::outfile;

  if (Global::quick) {
    cameraConfig.w_res /= 4;
    cameraConfig.h_res /= 4;
  }

  std::cout << ">>> Largura do 'Film': " << cameraConfig.w_res << "\n";
  std::cout << ">>> Altura do 'Film': " << cameraConfig.h_res << "\n";

  generalConfig.ppm_ = ps.retrieve<vec<std::string>>("img_type")[0] == "ppm";
}

void App::camera(const ParamSet &ps) {
  cameraConfig.type = ps.retrieve<vec<std::string>>("type")[0];

  /* Se houver 'screen_window_', damos preferência. */
  if (ps.has_elem("screen_window")) {
    auto sw = ps.retrieve<vec<ScreenWindow>>("screen_window",
                                             {{0.0, 0.0, 0.0, 0.0}})[0];
    cameraConfig.l_ = sw.l_;
    cameraConfig.r_ = sw.r_;
    cameraConfig.b_ = sw.b_;
    cameraConfig.t_ = sw.t_;
    return;
  }

  /* Se não, trabalhamos com 'fovy', 'frame_aspect_ratio' ou w_res/h_res. */
  cameraConfig.fovy = ps.retrieve<vec<int>>("fovy", {0})[0];
  cameraConfig.aspec = ps.retrieve<vec<float>>("frame_aspect_ratio", {0.0})[0];
}

void App::lookat(const ParamSet &ps) {
  cameraConfig.look_from =
      ps.retrieve<vec<point3>>("look_from", {{0, 0, 0}})[0];
  cameraConfig.look_at = ps.retrieve<vec<point3>>("look_at", {{0, 0, 0}})[0];
  cameraConfig.up = ps.retrieve<vec<point3>>("up", {{0, 0, 0}})[0];
}

void App::integrator(const ParamSet &ps) {
  generalConfig.integratorType = ps.retrieve<vec<std::string>>("type")[0];
  generalConfig.depth = (std::size_t)ps.retrieve<vec<int>>("depth")[0];
}

void triangleMesh(const ParamSet &ps) {

  std::vector<point3> vertices = ps.retrieve<std::vector<point3>>("vertices");

  std::vector<point3> indices = ps.retrieve<std::vector<point3>>("indices");

  std::vector<point3> normals = ps.retrieve<std::vector<point3>>("normals");

  std::vector<point2> uv = ps.retrieve<std::vector<point2>>("uv");
  std::cout << "IMPRIMINDO VERTICES:\n";
  for (auto &e : vertices)
    std::cout << e.str() << "\n";

  std::cout << "IMPRIMINDO INDICES:\n";
  for (auto &e : indices)
    std::cout << e.str() << "\n";

  std::cout << "IMPRIMINDO NORMALS:\n";
  for (auto &e : normals)
    std::cout << e.str() << "\n";

  std::cout << "IMPRIMINDO UV\n";
  for (auto &e : uv)
    std::cout << e.str() << "\n";
}
void App::object(const ParamSet &ps) {
  std::string type = ps.retrieve<vec<std::string>>("type")[0];

  if (type == "sphere") {
    sphere(ps);
  } else if (type == "plane") {
    plane(ps);
  } else if (type == "cube") {
    cube(ps);
  } else if (type == "pyramid") {
    pyramid(ps);
  } else if (type == "trianglemesh") {
    triangleMesh(ps);
  } else {
    std::cout << "Objeto " << (type.empty() ? "vazio" : type) << "inválido.\n";
  }
}

// Implementação do método que cria a esfera a partir do XML
void App::sphere(const ParamSet &ps) {
  point3 center = ps.retrieve<vec<point3>>("center", {{0, 0, 0}})[0];
  float radius = ps.retrieve<vec<float>>("radius", {1.0f})[0];

  std::cout << ">>> Criando 'Sphere' com raio " << radius << " e centro "
            << center.str() << ".\n";

  float z_min = ps.retrieve<vec<float>>("z_min", {-radius})[0];
  float z_max = ps.retrieve<vec<float>>("z_max", {radius})[0];
  float phi_max = ps.retrieve<vec<float>>("phi_max", {360.0f})[0];

  /* Instanciando um 'Shape' de Owner único (unique_ptr). */
  auto shape = std::make_unique<Sphere>(center, radius, z_min, z_max, phi_max);

  /* Instanciando um 'Material' de Owner compartilhado (shared_ptr). */
  std::shared_ptr<Material> mat = currMaterial;

  auto geoPrim = std::make_shared<GeometricPrimitive>(std::move(shape), mat);

  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::light_source(const ParamSet &ps) {
  auto type = ps.retrieve<vec<std::string>>("type")[0];
  auto intensity = ps.retrieve<vec<RGBColor>>("I", {{1, 1, 1}})[0];
  auto scale = ps.retrieve<vec<RGBColor>>("scale", {{1, 1, 1}})[0];

  if (type == "ambient") {
    sceneConfig.lights.push_back(
        std::make_shared<AmbientLight>(intensity, scale));
  } else if (type == "directional") {
    auto from = ps.retrieve<vec<point3>>("from", {0, 1, 0})[0];
    auto to = ps.retrieve<vec<point3>>("to", {0, 0, 0})[0];
    sceneConfig.lights.push_back(
        std::make_shared<DirectionalLight>(intensity, scale, from, to));
  } else if (type == "point") {
    auto from = ps.retrieve<vec<point3>>("from", {{0, 0, 0}})[0];
    sceneConfig.lights.push_back(
        std::make_shared<PointLight>(intensity, scale, from));
  } else if (type == "spot") {
    auto from = ps.retrieve<vec<point3>>("from", {{0, 0, 0}})[0];
    auto to = ps.retrieve<vec<point3>>("to", {{0, 0, 0}})[0];
    auto c = ps.retrieve<vec<int>>("cutoff", {50})[0];
    auto f = ps.retrieve<vec<int>>("falloff", {20})[0];

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

  point3 point = ps.retrieve<vec<point3>>("point", {{0, 0, 0}})[0];
  vec3 normal = ps.retrieve<vec<vec3>>("normal", {{0, 1, 0}})[0];
  auto shape = std::make_unique<Plane>(point, normal);
  auto geoPrim =
      std::make_shared<GeometricPrimitive>(std::move(shape), currMaterial);
  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::cube(const ParamSet &ps) {

  auto p1 = ps.retrieve<vec<point3>>("p1")[0];
  auto p2 = ps.retrieve<vec<point3>>("p2")[0];
  auto shape = std::make_unique<Cube>(p1, p2);
  auto geoPrim =
      std::make_shared<GeometricPrimitive>(std::move(shape), currMaterial);
  sceneConfig.aggrPrim->addObject(std::move(geoPrim));
}

void App::pyramid(const ParamSet &ps) {

  point3 center = ps.retrieve<vec<point3>>("center", {{0, 0, 0}})[0];
  float width = ps.retrieve<vec<float>>("width", {1.0f})[0];
  float height = ps.retrieve<vec<float>>("height", {1.0f})[0];
  auto shape = std::make_unique<Pyramid>(center, width, height);
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
