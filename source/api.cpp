#include "api.hpp"
#include "background.hpp"

#include <iostream>
void Api::backGround(const ParamSet &ps) {

  std::string v = ps.retrieve<std::string>("type", "");
  std::cout << v << "\n";

  RGBColor r = ps.retrieve<RGBColor>("color");
  std::cout << (int)r.R << " " << (int)r.G << " " << (int)r.B << "\n";
}
void Api::film(const ParamSet &ps) {

  std::string v = ps.retrieve<std::string>("type", "");
  std::cout << v << "\n";

  std::string v1 = ps.retrieve<std::string>("filename", "");
  std::cout << v1 << "\n";

  int v2 = ps.retrieve<int>("x_res", 0);
  std::cout << v2 << "\n";
}
