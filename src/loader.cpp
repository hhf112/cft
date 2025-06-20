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
#include <iostream>  // for std::cerr
#include <optional>  // for std::optional
#include <string>    // for std::string
#include <thread>    // for std::thread

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"


std::optional<status> Tester::load_bin() {
  int wstatus = 0;
    std::optional<status> failedrun = run_bin(wstatus);
    if (failedrun)  return failedrun.value();

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
    std::cerr << BRIGHT_YELLOW_FG << "child exit status: " << WEXITSTATUS(wstatus) << COLOR_END << '\n'; 
  if (m_runtime >= TIME_LIMIT.count()) m_timeLimit = warning::TLE;

  return {};
}


void close_pipe(int pipe_fd[2]) {
  close(pipe_fd[0]);
  close(pipe_fd[1]);
}

std::optional<status> Tester::run_bin(int& wstatus) {
  int pipe_fd[2];
  pipe(pipe_fd);
  if (pipe(pipe_fd) == -1) {
    perror("loader: could not create a pipe to child process");
    return status::PROCESSING_ERR;
  }

  posix_spawn_file_actions_t actions;
  int input_fd = open(INPUT_FILE, O_RDONLY);
  int output_fd = open(OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC);
  if (input_fd == -1 || output_fd == -1) {
    close_pipe(pipe_fd);
    perror("loader: failed to open input or output file");
    return status::PROCESSING_ERR;
  }
  if (posix_spawn_file_actions_init(&actions) != 0) {
    close_pipe(pipe_fd);
    close(input_fd);
    close(output_fd);
    perror("loader: file_actions_init failed");
    return status::PROCESSING_ERR;
  }

  auto cleanup = [&]() {
    m_loaded = 1;
    close(pipe_fd[1]);
    close(input_fd);
    close(output_fd);
    posix_spawn_file_actions_destroy(&actions);
  };

  if (posix_spawn_file_actions_adddup2(&actions, pipe_fd[0], STDIN_FILENO) !=
          0 ||
      posix_spawn_file_actions_adddup2(&actions, output_fd, STDOUT_FILENO) !=
          0) {
    cleanup();
    perror("loader: dup2 failed for input or output pipe");
    return status::PROCESSING_ERR;
  }

  pid_t binID;
  char* argv[] = {m_filename.data(), nullptr};
  auto start = std::chrono::high_resolution_clock::now();
  if (posix_spawn(&binID, m_filename.data(), &actions, NULL, argv, NULL) != 0) {
    perror("loader: posix failed");
    return m_result = status::PROCESSING_ERR;
  }
  close(pipe_fd[0]);

  if (splice(input_fd, NULL, pipe_fd[1], NULL, SIZE_MAX, 0) == -1) {
    cleanup();
    perror("loader: splice failed");
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
      waitpid(binID, nullptr, 0);
      return m_result = status::IDLENESS;
    }
    std::this_thread::sleep_for(POLLING_RATE);
  }

  if (waiting < 0) {
    perror("wait failed");
    m_loaded = -1;
    return m_result = status::PROCESSING_ERR;
  }
  auto end = std::chrono::high_resolution_clock::now();
  cleanup();
  m_runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

  return {};
}

