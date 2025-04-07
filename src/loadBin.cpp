//IDLENESS timelimit changed to 4 seconds.
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
#include <ratio>
#include <string>
#include <thread>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

//
std::optional<status> Tester::loadBin() {
  pid_t binID;
  char* args[] = {filename.data(), nullptr};

  auto disp = DISP_LOAD;
  auto frames = FR_BOXX;
  std::thread showAnim{spinnerBool, std::ref(disp), std::ref(frames),
                       std::ref(loaded)};

  int binStatus;
  auto start = std::chrono::high_resolution_clock::now();
  {
    if (posix_spawn(&binID, filename.data(), NULL, NULL, args, NULL) != 0) {
      perror("posix failed");
      return result = status::PROCESSING_ERR;
    }

    pid_t res;
    // check for idleness
    while ((res = waitpid(binID, &binStatus, WNOHANG)) == 0) {
      if (std::chrono::high_resolution_clock::now() - start >= idleLimit) {
        kill(binID, SIGKILL);
        loaded = -1;

        showAnim.join();

        waitpid(binID, nullptr, 0);  // reap it
        return result = status::IDLENESS;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    if (res < 0) {
      // may or may not add detailed error cases here.
      perror("wait failed");
      loaded = -1;

      showAnim.join();
      return result = status::PROCESSING_ERR;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  showAnim.join();

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

  if (runtime >= TIME_LIMIT.count()) timeLimit = warning::TLE;

  return {};
}
