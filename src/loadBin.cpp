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
#include "../util/include.hpp"

#define utch reinterpret_cast<const char*>

//
 std::optional<status> Tester::loadBin() {
  pid_t binID;
  char* args[] = {filename.data(), nullptr};

  std::cerr << BRIGHT_YELLOW_FG << "loading binary ..." << COLOR_END;

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

    //can put more error handling here ...
  if (!WIFEXITED(binStatus)) {
    if (WIFSIGNALED(binStatus)) {
      std::cerr << "child terminated by signal " << WTERMSIG(binStatus) << '\n';
    }
    return status::RUNTIME_ERR;
  } else {
    std::cerr << GREEN_FG << utch(u8"\r✅ binary exited successfully with exit code ")
              << WEXITSTATUS(binStatus) << COLOR_END << '\n';
  }

  runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

  if (runtime >= TIME_LIMIT) timeLimit = warning::TLE;

  loaded = 1;
  return {};
}
