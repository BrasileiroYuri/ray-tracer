#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

void help() {
  std::string message = R"(
    Usage: rt3 [<options>] <input_scene_file>
    )";
  std::cout << message << "\n";
}

int main(int argc, char **argv) {

  if (argc < 2) {
    help();
    return EXIT_SUCCESS;
  }

  Parser p(argv[2]);

  p.parse();

  return EXIT_SUCCESS;
}
