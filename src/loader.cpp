#include <fcntl.h>         // for open()
#include <signal.h>        // for kill(), signal macros
#include <spawn.h>         // for posix
#include <sys/sendfile.h>  // for sendfile()
#include <sys/types.h>     // for types
#include <sys/wait.h>      // for wait
#include <unistd.h>        // for pipe()

#include <chrono>  // for std::chrono
#include <cstdio>  // for perror()
#include <cstdlib>
#include <functional>  // for std::ref()
#include <iostream>    // for std::cerr
#include <optional>    // for std::optional
#include <string>      // for std::string
#include <thread>      // for std::thread

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

#define INPUT_FILE "input.txt"
using namespace std::chrono;

//
std::optional<status> Tester::loadBin() {
  auto disp = DISP_LOAD;
  auto frames = FR_BOXX;
  std::thread showAnim{spinnerBool, std::ref(disp), std::ref(frames),
                       std::ref(m_loaded)};

  int wstatus = 0;
  {
    int pipe_fd[2];
    pipe(pipe_fd);
    auto close_pipe = [&]() {
      close(pipe_fd[0]);
      close(pipe_fd[1]);
    };

    if (pipe(pipe_fd) == -1) {
      showAnim.join();
      perror("loader: could not create a pipe to child process");
      return status::PROCESSING_ERR;
    }
    posix_spawn_file_actions_t actions;

    int input_fd = open(INPUT_FILE, O_RDONLY);
    if (input_fd == -1) {
      showAnim.join();
      close_pipe();
      perror("loader: failed to open input file");
      return status::PROCESSING_ERR;
    }

    if (posix_spawn_file_actions_init(&actions) != 0) {
      showAnim.join();
      close_pipe();
      close(input_fd);
      perror("loader: file_actions_init failed");
      return status::PROCESSING_ERR;
    }

    auto cleanup = [&]() {
      m_loaded = 1;
      showAnim.join();
      close(pipe_fd[1]);
      close(input_fd);
      posix_spawn_file_actions_destroy(&actions);
    };

    if (posix_spawn_file_actions_adddup2(&actions, pipe_fd[0], STDIN_FILENO) !=
        0) {
      cleanup();
      perror("dup2 failed");
      return status::PROCESSING_ERR;
    }
    if (posix_spawn_file_actions_addclose(&actions, pipe_fd[1]) != 0) {
      cleanup();
      perror("close failed");
      return status::PROCESSING_ERR;
    }

    pid_t binID;
    char* argv[] = {m_filename.data(), nullptr};
    auto start = std::chrono::high_resolution_clock::now();
    if (posix_spawn(&binID, m_filename.data(), &actions, NULL, argv, NULL) !=
        0) {
      perror("loader: posix failed");
      return m_result = status::PROCESSING_ERR;
    }
    close(pipe_fd[0]);

    if (splice(input_fd, NULL, pipe_fd[1], NULL, SIZE_MAX, 0) == -1) {
      cleanup();
      perror("loader: sendfile failed");
      kill(binID, SIGKILL);
      waitpid(binID, nullptr, 0);
      return status::PROCESSING_ERR;
    }

    pid_t waiting;
    while ((waiting = waitpid(binID, &wstatus, WNOHANG)) == 0) {
      // will be implementing itimer for this.
      if (std::chrono::high_resolution_clock::now() - start >= IDLE_LIMIT) {
        kill(binID, SIGKILL);
        m_loaded = -1;
        showAnim.join();
        waitpid(binID, nullptr, 0);
        return m_result = status::IDLENESS;
      }
      std::this_thread::sleep_for(POLLING_RATE);
    }

    // close(input_fd);
    if (waiting < 0) {
      perror("wait failed");
      m_loaded = -1;
      showAnim.join();
      return m_result = status::PROCESSING_ERR;
    }
    auto end = std::chrono::high_resolution_clock::now();
    cleanup();
    m_runtime =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
  }

  if (WIFSIGNALED(wstatus)) {
    m_loaded = -1;
    std::cerr << "loader: child terminated by signal " << WTERMSIG(wstatus)
              << '\n';
    return status::RUNTIME_ERR;
  }
  if (WIFSTOPPED(wstatus)) {
    m_loaded = -1;
    std::cerr << "loader: child stopped by signal " << WSTOPSIG(wstatus)
              << '\n';
    return status::RUNTIME_ERR;
  }
  m_loaded = 1;
  if (WIFEXITED(wstatus))
    std::cerr <<  "exit status: " << WEXITSTATUS(wstatus) << '\n';
  if (m_runtime >= TIME_LIMIT.count()) m_timeLimit = warning::TLE;

  return {};
}
