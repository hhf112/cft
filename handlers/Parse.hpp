#pragma once

#include <array>
#include <cstring>
#include <iostream>

#include "../enums.hpp"
#include "../ansi_macros.hpp"

class Parse {
 private:
  query action;


 public:
  Parse(int argc, char* argv[]) {
    switch (argc) {
      case 2:
      case 3:
      default:
        std::cout << WHITE_ON_GREEN << "All tests passed!\n" COLOR_END;
        std::cout<< "verdict: You are diagnosed with retard! :D \n";
        exit(1);
    }
  }

  inline query whichAction() { return action; }
};
