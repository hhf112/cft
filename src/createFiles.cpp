
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "../tools/Init.hpp"

int Init::createFiles() {
  std::cerr << "Creating cleaning logs \n";

  std::ofstream cl{"cl", std::ios::out};

  if (!cl) {
    std::cerr << RED_FG << "failed!" << COLOR_END << '\n';
    return 1;
  }

  std::ifstream templateStream{templ, std::ios::in};
  if (!templateStream) {
    std::cerr << "ERR: Failed to load template \n";
    return 1;
  }

  cl << "rm input.txt out.txt output.txt report.txt cl ";
  for (int i = 0; i < files; i++) {
    std::string fl;
    fl += (char)(i + 'a');
    fl += ".cpp";
    cl << fl << " " << (char)(i + 'a') << " ";

    std::ofstream f{fl, std::ios::out};
    if (!f) {
      std::cerr << RED_FG
                << "ERR: Failed to create specified count of files. \n"
                << COLOR_END;
      return 1;
    }

    f << templateStream.rdbuf();
    if (f.fail()) {
      std::cerr << RED_FG
                << "Unknown error occured while copying the template. \n"
                << COLOR_END;
      return 1;
    }

    templateStream.seekg(0);
  }

  return 0;
}
