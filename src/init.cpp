#include "cft/init.hpp"

#include <filesystem>  // for std::filesystem
#include <fstream>     // for fstream
#include <iostream>    // for std::cerr
#include <string>      // for std::string

Init::Init(int argc, char *argv[]) {
  std::cout << "Flushing / creating IO files ...\n";
  if (!std::ofstream{"in.txt", std::ios::out} ||
      !std::ofstream{"output.txt", std::ios::out} ||
      !std::ofstream{"out.txt", std::ios::out} ||
      !std::ofstream{"report.txt", std::ios::out}) {
    m_status = 1;
    std::cerr << RED_FG << "init: unsuccessfull!\n" << COLOR_END;
    return;
  }

  m_files = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
  std::string repo = std::filesystem::canonical("/proc/self/exe")
                         .parent_path()
                         .parent_path()
                         .string();
  m_templ = repo + "/template.cpp";

  std::cout << "Loading template ... \n";
  std::ifstream templateF{m_templ, std::ios::in};
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
  std::ifstream templateStream{m_templ, std::ios::in};
  if (!templateStream) {
    std::cerr << "createfiles: ERR: Failed to load template \n";
    return 1;
  }
  cl << "rm in.txt out.txt output.txt report.txt cl logs.txt";
  for (int i = 0; i < m_files; i++) {
    std::string fl;
    fl += (char)(i + 'a');
    fl += ".cpp";
    cl << fl << " " << (char)(i + 'a') << " ";
    std::ofstream f{fl, std::ios::out};
    if (!f) {
      std::cerr
          << RED_FG
          << "createfiles: ERR: Failed to create specified count of files. \n"
          << COLOR_END;
      return 1;
    }
    f << templateStream.rdbuf();
    if (f.fail()) {
      std::cerr
          << RED_FG
          << "createfiles: Unknown error occured while copying the template. \n"
          << COLOR_END;
      return 1;
    }
    templateStream.seekg(0);
  }
  cl << "> /dev/null 2>&1";
  std::cout << GREEN_FG << "Files created successfully! ;)" << COLOR_END
            << '\n';
  return 0;
}
