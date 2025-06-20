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
  std::string sourcefile = m_filename + ".cpp";
  char* args[] = {(char*)"g++", (char*)sourcefile.c_str(), (char*)"-o",
                  (char*)m_filename.c_str(), NULL};

  pid_t buildID;
  if (posix_spawnp(&buildID, "g++", NULL, NULL, args, environ) != 0) {
    perror("build: posix failed");
    return buildErr::PROCESSING_ERR;
  }

  int status;
  if (waitpid(buildID, &status, 0) < 0) {
    perror("build: wait failed");
    return buildErr::PROCESSING_ERR;
  }

  if (WIFEXITED(status))
    std::cerr << BRIGHT_YELLOW_FG << "build exit status: " <<  WEXITSTATUS(status) << COLOR_END << '\n';
  else {
    std::cerr << "build: unknown error occured\n" << '\n';
    return buildErr::PROCESSING_ERR;
  }

  if (m_ship) {
    std::fstream readsrc{sourcefile, std::ios::in};
    std::fstream cp{m_ship.value() + "ship.cpp", std::ios::out};

    if (!readsrc || !cp)
      std::cerr << "Unable to ship!\n";
    else {
      cp << readsrc.rdbuf();
      std::cerr << GREEN_FG << "\t shipped!\n";
    }
  }

  return {};
}
