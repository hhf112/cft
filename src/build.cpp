#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

extern char** environ;
std::optional<buildErr> Tester::build() {
  std::string sourcefile = filename + ".cpp";
  char* args[] = {(char*)"g++", (char*)sourcefile.c_str(), (char*)"-o",
                  (char*)filename.c_str(), NULL};

  pid_t buildID;
  if (posix_spawnp(&buildID, "g++", NULL, NULL, args, environ) != 0) {
    perror("posix failed");
    return buildErr::PROCESSING_ERR;
  }

  int status;
  if (waitpid(buildID, &status, 0) < 0) {
    perror("wait failed");
    return buildErr::PROCESSING_ERR;
  }

  if (!WIFEXITED(status)) {
    return buildErr::BUILD_FAIL;
  }

  if (ship) {
    std::fstream readsrc{sourcefile, std::ios::in};
    std::fstream cp{ship.value() + "ship.cpp", std::ios::out};

    if (!readsrc || !cp)
      std::cerr << "Unable to ship!\n";
    else {
      cp << readsrc.rdbuf();
      std::cerr << GREEN_FG << "\t shipped!\n";
    }
  }

  return {};
}
