#include "api.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>
#include <math.cpp>
#include <string>

void help() {

  std::string message = R"(
  Usage: rt3 [<options>] <input_scene_file>
      Rendering simulation options:
          --help                     Print this help text.
          --cropwindow <x0,x1,y0,y1> Specify an image crop window.
          --quick                    Reduces quality parameters to render image quickly.
          --outfile <filename>       Write the rendered image to <filename>.
)";

  std::cout << message << "\n";
}

int main(int argc, char **argv) {
  if (argc == 1 || (std::string)argv[1] == "--help")
    help();

  Parser p("");
  Api::render();
  return EXIT_SUCCESS;
}
