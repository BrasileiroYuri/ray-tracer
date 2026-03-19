#include "parser.hpp"
#include "tinyxml2.h"
#include <iostream>

void Parser::parse() const {
  tinyxml2::XMLDocument doc;

  if (doc.LoadFile(filename_.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << "Error loading the XML file!" << '\n';
    return;
  }
  auto root = doc.FirstChildElement("RT3");

  if (!root) {
    std::cerr << "Root node of the XML tree was not found!" << '\n';
    return;
  }

  for (auto it = root->FirstChildElement(); it; it = it->NextSiblingElement()) {

    if (elements_.find(it->Name()) == elements_.end()) {
      std::cerr << "Element \'" << it->Name() << "\' invalid.\n";
      continue;
    }

    ParamSet ps;

    for (auto it1 = it->FirstAttribute(); it1; it1 = it1->Next()) {
      if (conversor_.find(it1->Name()) == conversor_.end()) {
        std::cerr << "Attribute \'" << it1->Name() << "\' invalid.\n";
        continue;
      }

      conversor_.at(it1->Name())(it1->Name(), it1->Value(), &ps);
    }

    elements_.at(it->Name())(ps);
  }
}
