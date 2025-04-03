#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"

std::optional<buildErr> Tester::build() {
  std::string sourcefile = filename + ".cpp";
  char* args[] = {(char*)"g++", sourcefile.data(), (char*)"-o", filename.data(),
                  NULL};

  std::cerr << BRIGHT_YELLOW_FG << "building ..." << COLOR_END << '\n';
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

  return {};
}
