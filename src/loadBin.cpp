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
#include <thread>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

//
std::optional<status> Tester::loadBin() {
  pid_t binID;
  char* args[] = {filename.data(), nullptr};

  if (posix_spawn(&binID, filename.data(), NULL, NULL, args, NULL) != 0) {
    perror("posix failed");
    return result = status::PROCESSING_ERR;
  }

  auto disp = DISP_LOAD;
  auto frames = FR_BOXX;
  std::thread showAnim{spinnerBool, std::ref(disp), std::ref(frames), std::ref(loaded)};

  int binStatus;
  auto start = std::chrono::high_resolution_clock::now();
  if (waitpid(binID, &binStatus, 0) < 0) {
    loaded = -1;
    // may or may not add detailed error cases here.
    perror("wait failed");
    return result = status::PROCESSING_ERR;
  }
  auto end = std::chrono::high_resolution_clock::now();

  // can put more error handling here ...
  if (!WIFEXITED(binStatus)) {
    loaded = -1;
    if (WIFSIGNALED(binStatus)) {
      std::cerr << "child terminated by signal " << WTERMSIG(binStatus) << '\n';
    }
    return status::RUNTIME_ERR;
  } else {
        std::cerr << WEXITSTATUS(binStatus) << '\n';
    loaded = 1;
  }

  runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

  showAnim.join();

  if (runtime >= TIME_LIMIT) timeLimit = warning::TLE;

  return {};
}
