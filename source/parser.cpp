#include "parser.hpp"
#include "app.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <vector>

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
          attr_name == "diffuse" || attr_name == "specular" || attr_name == "I" || 
          attr_name == "scale") {
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
          attr_name == "diffuse" || attr_name == "specular" || attr_name == "I" || 
          attr_name == "scale") {
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
