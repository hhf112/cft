#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class Init {
 private:
  int files = 1;
  int status = 1;

  const std::string curdir;
  std::string cleanfs;
  std::string templ;


 public:
  Init(int argc, char* argv[], const std::string& cwd) : curdir{cwd} {
    // clear IO files.
    {
      std::ofstream c1{curdir + "/input.txt", std::ios::out};
      std::ofstream c2{curdir + "/output.txt", std::ios::out};
      std::ofstream c3{curdir + "/out.txt", std::ios::out};
      std::ofstream c4{curdir + "/report.txt", std::ios::out};

      if (!(c1 && c2 && c4 && c4)) {
        status = 0;
        return;
      }
    }

    // no of problem statements in the contest.
    files = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
    //
    // load template.
    std::filesystem::path binpath =
        std::filesystem::canonical("/proc/self/exe");
    templ = 
        binpath.parent_path() / "/template.txt";

    std::ifstream templateF{templ, std::ios::in};
    if (!templateF) {
      std::cerr << "Template not found!\n";
      return;
    }
  }

  int createFiles() {
    cleanfs += "rm ";
    for (char i = 'a'; i < files; i++) {
      std::string num;
      num += curdir + "/" + i + ".cpp";
      cleanfs += num + " " + i + " ";

      std::ofstream f{num, std::ios::out};
      f << templ;
    }

    return 0;
  }

  bool queryCleanup() {
    std::ofstream clean{curdir + "/cl", std::ios::out};
    if (!clean) {
      std::cerr << "Could not create cleaning logs\n";
      return 1;
    }
    clean << cleanfs;
    clean.close();

    return 0;
  }

  int updateTemplate() { return 0; }
};
