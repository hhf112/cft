#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


#include "../ansi_macros.hpp"

class Init {
 private:
  int files = 1;
  int status = 0;

  const std::string curdir;
  std::string cleanfs;
  std::string templ;

 public:
  Init(int argc, char* argv[], const std::string& cwd) : curdir{std::move(cwd)} {
    std::cout << "Flushing IO files ...\n";
    // clear IO files.
    {
      std::ofstream c1{curdir + "/input.txt", std::ios::out};
      std::ofstream c2{curdir + "/output.txt", std::ios::out};
      std::ofstream c3{curdir + "/out.txt", std::ios::out};
      std::ofstream c4{curdir + "/report.txt", std::ios::out};

      if (!(c1 && c2 && c4 && c4)) {
        status = 1;
        std::cerr << RED_FG << "Unsuccessfull\n" << COLOR_END;
        return;
      }
    }

    // no of problem statements in the contest.
    files = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
    //
    // load template.
    std::filesystem::path binpath =
        std::filesystem::canonical("/proc/self/exe");
    templ = binpath.parent_path() / "/template.txt";

    std::cout << "Loading template\n";
    std::ifstream templateF{templ, std::ios::in};
    if (!templateF) {
      std::cerr << RED_FG << "Template not found!\n" << COLOR_END;
      return;
    }
  }

  inline int createFiles() {
    cleanfs += "rm ";

    std::ifstream templateStream{templ, std::ios::in};
    if (!templateStream) {
      std::cerr << "ERR: Failed to load template \n";
      return 1;
    }

    for (char i = 'a'; i < files; i++) {
      std::string num;
      num += curdir + "/" + i + ".cpp";
      cleanfs += num + " " + i + " ";

      std::ofstream f{num, std::ios::out};
      if (!f) {
        std::cerr << RED_FG <<  "ERR: Failed to create specified count of files. \n" << COLOR_END;
        return 1;
      }
      // f << templ;
    }

    return 0;
  }

  inline int queryCleanup() {
    std::ofstream clean{curdir + "/cl", std::ios::in};
    if (!clean) {
      std::cerr << "Could not create cleaning logs\n";
      return 1;
    }
    clean << cleanfs;

    return 0;
  }

  inline int configTempl(){

    return 0;
  }
};
