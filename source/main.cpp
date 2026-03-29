#include "app.hpp"
#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// Namespace para comunicar as opções com os outros arquivos (.cpp)
namespace Global {
std::string outfile = "";
bool quick = false;
int crop[4] = {0, 0, 0, 0};
bool has_crop = false;
} // namespace Global

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
  if (argc < 2) {
    help();
    return EXIT_FAILURE;
  }

  std::string scene_file = "";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--help") {
      help();
      return EXIT_SUCCESS;
    } else if (arg == "--outfile" && i + 1 < argc) {
      Global::outfile = argv[++i];
    } else if (arg == "--quick") {
      Global::quick = true;
    } else if (arg == "--cropwindow" && i + 1 < argc) {
      std::string window = argv[++i];
      // Tenta ler os 4 valores inteiros separados por vírgula
      if (sscanf(window.c_str(), "%d,%d,%d,%d", &Global::crop[0],
                 &Global::crop[1], &Global::crop[2], &Global::crop[3]) == 4) {
        Global::has_crop = true;
      }
    } else if (arg[0] != '-') {
      // Se não começa com '-', assumimos que é o caminho do arquivo XML
      scene_file = arg;
    }
  }

  if (scene_file.empty()) {
    std::cerr << "Erro: Nenhum arquivo de cena especificado.\n";
    help();
    return EXIT_FAILURE;
  }

  Parser p(scene_file);

  p.parse();

  return EXIT_SUCCESS;
}
