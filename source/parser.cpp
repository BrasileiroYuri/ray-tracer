#include "parser.hpp"
#include "app.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
static void convert(const std::string &name, const std::string &value,
                    ParamSet *ps) {
  std::istringstream ss{value};
  T val{};
  ss >> val;
  ps->add(name, val);
}

template <typename T, typename K, std::size_t size>
static void convert(const std::string &name, const std::string &value,
                    ParamSet *ps) {
  std::istringstream ss{value};
  std::array<K, size> arr;
  for (unsigned long i = 0; i < size; i++)
    ss >> arr[i];
  T val{arr};
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

  conversor_ = {
      {"frame_aspect_ratio", convert<float>},
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
      {"type", convert<std::string>},
      {"name", convert<std::string>},
      {"filename", convert<std::string>},
      {"img_type", convert<std::string>},
      {"mapping", convert<std::string>},
      {"bl", convert<RGBColor, int, 3>},
      {"br", convert<RGBColor, int, 3>},
      {"tl", convert<RGBColor, int, 3>},
      {"tr", convert<RGBColor, int, 3>},
      {"single_color", convert<RGBColor, int, 3>},
      {"color", convert<RGBColor, int, 3>},
      {"look_from", convert<point3, float, 3>},
      {"look_at", convert<point3, float, 3>},
      {"plane", convert<point3, float, 3>},
      {"center", convert<point3, float, 3>},
      {"mirror", convert<RGBColor, float, 3>},
      {"normal", convert<vec3, float, 3>},
      {"up", convert<vec3, float, 3>},
      {"screen_window", convert<ScreenWindow, float, 4>},
      {"ambient", convert<RGBColor, float, 3>},
      {"diffuse", convert<RGBColor, float, 3>},
      {"specular", convert<RGBColor, float, 3>},
      {"I", convert<RGBColor, float, 3>},
      {"scale", convert<RGBColor, float, 3>},
      {"from", convert<point3, float, 3>},
      {"to", convert<point3, float, 3>},
      {"p1", convert<point3, float, 3>},
      {"p2", convert<point3, float, 3>},
      {"glossiness", convert<float>},
  };
}
RGBColor parse_color_format(const std::string &str) {
  std::stringstream ss(str);
  std::vector<float> v;
  float val;
  while (ss >> val)
    v.push_back(val);

  if (v.size() < 3)
    return RGBColor(0, 0, 0);

  // Verifica se o usuário está usando a escala 0-255
  bool is_255_scale = false;
  for (float c : v) {
    if (c > 1.0f) {
      is_255_scale = true;
      break;
    }
  }

  // Converte para float 0.0-1.0 se necessário e retorna o RGBColor
  if (is_255_scale) {
    return RGBColor(v[0] / 255.0f, v[1] / 255.0f, v[2] / 255.0f);
  }
  return RGBColor(v[0], v[1], v[2]);
}

void Parser::include(const std::string &filename) const {
  tinyxml2::XMLDocument doc;

  if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << ">>> Error loading the '" << filename
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

    ParamSet ps;
    for (auto attr = it->FirstAttribute(); attr; attr = attr->Next()) {
      std::string attr_name = attr->Name();
      std::string attr_val = attr->Value();

      //@TODO: Refazer, não escala.
      // 1. Processamento de Cores e intensidades
      if (attr_name == "color" || attr_name == "bl" || attr_name == "tl" ||
          attr_name == "tr" || attr_name == "br" || attr_name == "ambient" ||
          attr_name == "diffuse" || attr_name == "specular" ||
          attr_name == "I" || attr_name == "scale") {
        ps.add(attr_name, parse_color_format(attr_val));
      }

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
    } else if (name == "include") {
      std::string attr = it->FirstAttribute()->Name();

      if (attr == "filename") {
        include(it->FirstAttribute()->Value());
        continue;
      } else {
        std::cout << ">>> Atributo '" << attr
                  << "' from tag 'include' é inválido.\n";
      }
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

      //@TODO: Refazer, não escala.
      // 1. Processamento de Cores e intensidades
      if (attr_name == "color" || attr_name == "bl" || attr_name == "tl" ||
          attr_name == "tr" || attr_name == "br" || attr_name == "ambient" ||
          attr_name == "diffuse" || attr_name == "specular" ||
          attr_name == "I" || attr_name == "scale") {
        ps.add(attr_name, parse_color_format(attr_val));
      }

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
