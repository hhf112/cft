#include <fstream>
#include <iostream>

#define INPUT 0
#define OUTPUT 1
#define OUT 2
#define REPORT 3

class cfg {
public: 
  std::string io_files[4];

  cfg() {
    std::fstream config {"config.txt", std::ios::in};
    if (!config) {
      std::cerr << "failed to load config\n";
      exit(1);
    }

    int line = 0;
    while (line < 4 && std::getline(config, io_files[line])) ++line;
  }
};
