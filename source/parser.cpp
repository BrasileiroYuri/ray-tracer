#include "parser.hpp"
#include "tinyxml2.h"
#include "api.hpp"
#include <iostream>

void Parser::parse() const {
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filename_.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Erro ao carregar arquivo XML: " << filename_ << std::endl;
        return;
    }

    auto root = doc.FirstChildElement("RT3");
    if (!root) return;

    for (auto it = root->FirstChildElement(); it; it = it->NextSiblingElement()) {
        std::string name = it->Name();

        // Requisito: disparar render ao encontrar world_end
        if (name == "world_end") {
            Api::render();
            continue;
        }

        if (elements_.find(name) == elements_.end()) continue;

        ParamSet ps;
        for (auto it1 = it->FirstAttribute(); it1; it1 = it1->Next()) {
            if (conversor_.find(it1->Name()) != conversor_.end()) {
                conversor_.at(it1->Name())(it1->Name(), it1->Value(), &ps);
            }
        }
        elements_.at(name)(ps);
    }
}