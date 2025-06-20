#include <fcntl.h>         // for open()
#include <spawn.h>         // for posix
#include <sys/sendfile.h>  // for sendfile()
#include <sys/types.h>     // for types
#include <sys/wait.h>      // for wait
#include <unistd.h>        // for pipe()

#include <chrono>    // for std::chrono
#include <csignal>   // for std::signal
#include <cstdio>    // for perror()
#include <cstring>   // for strsignal
#include <iostream>  // for std::cerr
#include <optional>  // for std::optional
#include <string>    // for std::string

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

// right now as macros. configuration will be added.
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

int bin_id;
void handler(int sig_id) {
  std::cerr << BRIGHT_YELLOW_FG << "IDLENESS TIME LIMIT EXCEEDED!" << COLOR_END
            << '\n';
  kill(bin_id, SIGKILL);
}

std::optional<status> Tester::load_bin() {
  int wstatus = 0;
  std::optional<status> failedrun = run_bin(wstatus);
  if (failedrun) return failedrun.value();

  if (WIFSIGNALED(wstatus)) {
    m_loaded = -1;
    std::cerr << RED_FG << "child terminated by signal "
              << strsignal(WTERMSIG(wstatus)) << COLOR_END << '\n';
    return status::RUNTIME_ERR;
  }
  if (WIFSTOPPED(wstatus)) {
    m_loaded = -1;
    std::cerr << RED_FG << "child stopped by signal "
              << strsignal(WSTOPSIG(wstatus)) << COLOR_END << '\n';
    return status::RUNTIME_ERR;
  }
  m_loaded = 1;
  if (WIFEXITED(wstatus))
    std::cerr << BRIGHT_YELLOW_FG
              << "child exit status: " << WEXITSTATUS(wstatus) << COLOR_END
              << '\n';
  if (m_runtime >= TIME_LIMIT.count()) m_timeLimit = warning::TLE;

  return {};
}

void close_pipe(int pipe_fd[2]) {
  close(pipe_fd[0]);
  close(pipe_fd[1]);
}

std::optional<status> Tester::run_bin(int& wstatus) {
  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    perror("loader: could not create a pipe to child process");
    return status::PROCESSING_ERR;
  }

  int input_fd =
      open(INPUT_FILE, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  int output_fd = open(OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (input_fd == -1 || output_fd == -1) {
    close_pipe(pipe_fd);
    perror("loader: failed to open input or output file");
    return status::PROCESSING_ERR;
  }

  posix_spawn_file_actions_t actions;
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

  char* argv[] = {m_filename.data(), nullptr};
  pid_t waiting;
  auto start = std::chrono::high_resolution_clock::now();
  if (posix_spawn(&bin_id, m_filename.data(), &actions, NULL, argv, NULL) !=
      0) {
    perror("loader: posix failed");
    m_loaded = -1;
    return m_result = status::PROCESSING_ERR;
  }
  close(pipe_fd[0]);

  // idleness
  alarm(IDLE_LIMIT.count());
  signal(SIGALRM, handler);

  if (splice(input_fd, NULL, pipe_fd[1], NULL, SIZE_MAX, 0) == -1) {
    cleanup();
    perror("loader: splice failed");
    kill(bin_id, SIGKILL);
    waitpid(bin_id, nullptr, WNOHANG);
    return status::PROCESSING_ERR;
  }

  if (waitpid(bin_id, &wstatus, 0) < 0) {
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
