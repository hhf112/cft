#pragma once

#include <cstring>
#include <iostream>

#include "../enums.hpp"

class Parse {
 private:
  query action;

 public:
  Parse(int argc, char* argv[]) {
    switch (argc) {
      case 2:
      case 3:
        if (strcmp(argv[1], "init"))
          action = query::init;
        else if (strcmp(argv[1], "template"))
          action = query::config;
        else
          action = query::judge;
        break;
      default:
        std::cout << "\033[97;42mAll tests passed!\033[0m\n verdict: You are "
                     "diagnosed with retard! :D \n";
        exit(1);
    }
  }

  query whichAction (){
    return action;
  }
};
