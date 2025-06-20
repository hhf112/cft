
#include "../tools/Init.hpp"

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

//create files, check availibility of template.
Init::Init(int argc, char *argv[]) {
  std::cout << "Flushing / creating IO files ...\n";
  // clear IO files.
  if (!std::ofstream{"input.txt", std::ios::out} ||
      !std::ofstream{"output.txt", std::ios::out} ||
      !std::ofstream{"out.txt", std::ios::out} ||
      !std::ofstream{"report.txt", std::ios::out}) {
    status = 1;
    std::cerr << RED_FG << "init: unsuccessfull!\n" << COLOR_END;
    return;
  }

  // no of problem statements in the contest.
  files = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
  //
  // load template.
  std::string repo = std::filesystem::canonical("/proc/self/exe")
                         .parent_path()
                         .parent_path()
                         .string();
  templ = repo + "/template.cpp";

  std::cout << "Loading template ... \n";
  std::ifstream templateF{templ, std::ios::in};
  if (!templateF) {
    std::cerr << RED_FG << "init: template not found!" << COLOR_END << '\n';
    return;
  }
}

int Init::createFiles() {
  std::cerr << "Creating cleaning logs \n";
  std::ofstream cl{"cl", std::ios::out};
  if (!cl) {
    std::cerr << RED_FG << "failed!" << COLOR_END << '\n';
    return 1;
  }
  std::ifstream templateStream{templ, std::ios::in};
  if (!templateStream) {
    std::cerr << "createfiles: ERR: Failed to load template \n";
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
                << "createfiles: ERR: Failed to create specified count of files. \n"
                << COLOR_END;
      return 1;
    }
    f << templateStream.rdbuf();
    if (f.fail()) {
      std::cerr << RED_FG
                << "createfiles: Unknown error occured while copying the template. \n"
                << COLOR_END;
      return 1;
    }
    templateStream.seekg(0);
  }
    cl << "> /dev/null 2>&1";
    std::cout << GREEN_FG << "Files created successfully! ;)" << COLOR_END << '\n';
  return 0;
}
