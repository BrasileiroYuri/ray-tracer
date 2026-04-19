#include "parser.hpp"
#include "app.hpp"
#include "sphere.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <vector>

// Mantive a lógica de processamento de cores
RGBColor parse_color_format(const std::string &str) {
  std::stringstream ss(str);
  std::vector<float> v;
  float val;
  while (ss >> val)
    v.push_back(val);

  if (v.size() < 3)
    return RGBColor(0, 0, 0);

  bool is_normalized = true;
  for (float c : v) {
    if (c > 1.0f) {
      is_normalized = false;
      break;
    }
  }

  if (is_normalized) {
    return RGBColor((unsigned char)(v[0] * 255), (unsigned char)(v[1] * 255),
                    (unsigned char)(v[2] * 255));
  }
  return RGBColor((unsigned char)v[0], (unsigned char)v[1],
                  (unsigned char)v[2]);
}

void Parser::parse() const {
  tinyxml2::XMLDocument doc;

  if (doc.LoadFile(filename_.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << "Error loading the XML file!" << '\n';
    return;
  }

  auto root = doc.FirstChildElement("RT3");
  if (!root)
    return;

  for (auto it = root->FirstChildElement(); it; it = it->NextSiblingElement()) {
    std::string name = it->Name();

    // Ignora marcadores de escopo
    if (name == "world_begin") {
      continue;
    }

    // Dispara a renderização ao final do arquivo
    if (name == "world_end") {
      App::render();
      break;
    }

    // Verifica se a tag (ex: camera, sphere, background) existe no dicionário
    if (elements_.find(name) == elements_.end()) {
#ifdef DEBUG
      std::cerr << "Element '" << name << "\' invalid.\n";
#endif
      continue;
    }

    ParamSet ps;
    for (auto attr = it->FirstAttribute(); attr; attr = attr->Next()) {
      std::string attr_name = attr->Name();
      std::string attr_val = attr->Value();

      // 1. Processamento de Cores
      if (attr_name == "color" || attr_name == "bl" || attr_name == "tl" ||
          attr_name == "tr" || attr_name == "br") {
        ps.add(attr_name, parse_color_format(attr_val));
      }

      if (conversor_.find(attr_name) == conversor_.end()) {
#ifdef DEBUG
        std::cerr << "Atribute: '" << attr_name << "Invalid.\n";
#endif
        continue;
      }

      conversor_.at(attr_name)(attr_name, attr_val, &ps);
    }

    // Executa a função associada à tag (ex: criar esfera, configurar câmera)
    elements_.at(name)(ps);
    ps.remove();
  }
}
