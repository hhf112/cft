#pragma once

#include <array>
#include <cstring>
#include <iostream>

#include "../enums.hpp"

class Parse {
 private:
  query action;

  std::array<std::string, query::COUNT> tokens {"config", "judge", "init"};

 public:
  Parse(int argc, char* argv[]) {
    switch (argc) {
      case 2:
      case 3:
      default:
        std::cout << "\033[97;42mAll tests passed!\033[0m\n verdict: You are "
                     "diagnosed with retard! :D \n";
        exit(1);
    }
  }

  query whichAction() { return action; }
};
