
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
