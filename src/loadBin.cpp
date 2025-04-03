#include <signal.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"

//
inline std::optional<status> Tester::loadBin() {
  // runtime
  std::cerr << BRIGHT_YELLOW_FG << "loading binary ..." << COLOR_END << '\n';
  pid_t binID;
  char* args[] = {filename.data(), nullptr};
  if (posix_spawn(&binID, filename.data(), NULL, NULL, args, NULL) != 0) {
    perror("posix failed");
    return result = status::PROCESSING_ERR;
  }
  auto start = std::chrono::high_resolution_clock::now();

  int binStatus;
  if (waitpid(binID, &binStatus, 0) < 0) {
    // may or may not add detailed error cases here.
    perror("wait failed");
    return result = status::PROCESSING_ERR;
  }
  auto end = std::chrono::high_resolution_clock::now();

  if (!WIFEXITED(binStatus)) {
    if (WIFSIGNALED(binStatus)) {
      std::cerr << "child terminated by signal " << WTERMSIG(binStatus) << '\n';
    }
    return status::RUNTIME_ERR;
  } else {
    std::cerr << "\tchild exited succesfully with exit code "
              << WEXITSTATUS(binStatus) << '\n';
  }

  runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

  if (runtime >= 150) timeLimit = warning::TLE;

  loaded = 1;
  return {};
}
