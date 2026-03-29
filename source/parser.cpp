#include "parser.hpp"
#include "app.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <vector>

// Função para tratar os dois formatos de cor
RGBColor parse_color_format(const std::string &str) {
  std::stringstream ss(str);
  std::vector<float> v;
  float val;
  while (ss >> val)
    v.push_back(val);

  if (v.size() < 3)
    return RGBColor(0, 0, 0);

  // Verifica se algum valor ultrapassa 1.0 para determinar a escala
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

    if (name == "world_end") {
      App::render();
      continue;
    }

    if (elements_.find(name) == elements_.end()) {
      std::cerr << "Element \'" << name << "\' invalid.\n";
      continue;
    }

    ParamSet ps;
    for (auto attr = it->FirstAttribute(); attr; attr = attr->Next()) {
      std::string attr_name = attr->Name();
      std::string attr_val = attr->Value();

      // Suporte a cores [0,1] ou [0,255]
      if (attr_name == "color" || attr_name == "bl" || attr_name == "tl" ||
          attr_name == "tr" || attr_name == "br") {
        ps.add(attr_name, parse_color_format(attr_val));
      } else if (conversor_.find(attr_name) != conversor_.end()) {
        conversor_.at(attr_name)(attr_name, attr_val, &ps);
      }
    }

    elements_.at(name)(ps);
  }
}
