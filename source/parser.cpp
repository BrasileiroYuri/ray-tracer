#include "parser.hpp"
#include "tinyxml2.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

template <typename T>
void Parser::convert(const std::string &name, const std::string &value,
                     ParamSet *ps) {

  auto ptr = dynamic_cast<T>(value);

  if (!ptr) {
    std::cout << "bad_cast\n";
    exit(1);
  }

  ps->add(name, value);
}

template <typename T, std::size_t size>
void Parser::convert(const std::string &name, const std::string &value,
                     ParamSet *ps) {}

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

    if (elements_.find(it->Name()) ==
        elements_.end()) // Se não é elemento que estamos esperando (temos
                         // função) vamos pro próximo.
      continue;

    ParamSet *ps = new ParamSet();

    for (auto it1 = it->FirstAttribute(); it1; it1 = it1->Next())
      conversor_.at(it1->Name())(it1->Name(), it1->Value(), ps);

    elements_.at(it->Name())(*ps);
  }
}
