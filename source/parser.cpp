#include "parser.hpp"
#include "app.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>

/* Método de conversão feito para ser simples. */
template <typename T>
static void convert(const std::string &name, const std::string &value,
                    ParamSet *ps) {

  std::istringstream ss{value};
  T val{};

  while (ss >> val)
    ps->add(name, val);
}

//!< Def:
Parser::Parser(const std::string &filename) : filename_(filename) {
  elements_ = {
      {"film", App::film},
      {"background", App::backGround},
      {"camera", App::camera},
      {"lookat", App::lookat},
      {"object", App::object},
      {"make_named_material", App::make_named_material},
      {"named_material", App::named_material},
      {"material", App::material},
      {"integrator", App::integrator},
      {"light_source", App::light_source},
  };

  conversor_ = {{"frame_aspect_ratio", convert<float>},
                {"radius", convert<float>},
                {"z_min", convert<float>},
                {"z_max", convert<float>},
                {"phi_max", convert<float>},
                {"w_res", convert<int>},
                {"h_res", convert<int>},
                {"fovy", convert<int>},
                {"cutoff", convert<int>},
                {"depth", convert<int>},
                {"falloff", convert<int>},
                {"ntriangles", convert<int>},
                {"type", convert<std::string>},
                {"name", convert<std::string>},
                {"filename", convert<std::string>},
                {"img_type", convert<std::string>},
                {"mapping", convert<std::string>},
                {"material", convert<std::string>},
                {"bl", convert<RGBColor>},
                {"br", convert<RGBColor>},
                {"tl", convert<RGBColor>},
                {"tr", convert<RGBColor>},
                {"single_color", convert<RGBColor>},
                {"color", convert<RGBColor>},
                {"look_from", convert<point3>},
                {"look_at", convert<point3>},
                {"plane", convert<point3>},
                {"center", convert<point3>},
                {"mirror", convert<RGBColor>},
                {"normal", convert<vec3>},
                {"up", convert<vec3>},
                {"screen_window", convert<ScreenWindow>},
                {"ambient", convert<RGBColor>},
                {"diffuse", convert<RGBColor>},
                {"specular", convert<RGBColor>},
                {"I", convert<vec3>},
                {"scale", convert<vec3>},
                {"from", convert<point3>},
                {"to", convert<point3>},
                {"p1", convert<point3>},
                {"p2", convert<point3>},
                {"glossiness", convert<float>},
                {"width", convert<float>},
                {"height", convert<float>},
                {"vertices", convert<point3>},
                {"indices", convert<point3>},
                {"normals", convert<point3>},
                {"uv", convert<point2>}};
}

void Parser::parse() const {
  tinyxml2::XMLDocument doc;

  if (doc.LoadFile(filename_.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << ">>> Error loading the '" << filename_
              << "' XML file!\nError:" << '\n';
    doc.PrintError();
    return;
  }

  auto root = doc.FirstChildElement("RT3");
  if (!root)
    return;

  for (auto it = root->FirstChildElement(); it; it = it->NextSiblingElement()) {
    std::string name = it->Name();

    if (name == "world_begin") {
      continue;
    } else if (name == "world_end" || name == "render_again") {
      App::render();
      continue;
    }

    // Verifica se a tag (ex: camera, sphere, background) existe no dicionário
    if (elements_.find(name) == elements_.end()) {
      std::cerr << "Tag: '" << name << "' inválida.\n";
      continue;
    }

    ParamSet ps;
    for (auto attr = it->FirstAttribute(); attr; attr = attr->Next()) {
      std::string attr_name = attr->Name();
      std::string attr_val = attr->Value();

      if (conversor_.find(attr_name) == conversor_.end()) {
        std::cerr << "Atribute: '" << attr_name << "' invalid.\n";
        continue;
      }

      conversor_.at(attr_name)(attr_name, attr_val, &ps);
    }

    // Executa a função associada à tag (ex: criar esfera, configurar câmera)
    elements_.at(name)(ps);
  }
}
