#include "parser.hpp"

#include <cstdlib>
#include <iostream>
#include <math.hpp>
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

/*void options(int argc, char **argv) {
  App::RunningOptions op;
}*/

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage " << argv[0] << " <scene.xml>\n";
    return EXIT_SUCCESS;
  }

  Parser p(argv[1]);
  p.parse();
  App::render();
  return EXIT_SUCCESS;
}
