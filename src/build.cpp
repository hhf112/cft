#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

std::optional<buildErr> Tester::build() {
  std::string sourcefile = filename + ".cpp";
  char* args[] = {(char*)"g++", sourcefile.data(), (char*)"-o", filename.data(),
                  NULL};

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

  return {};
}
