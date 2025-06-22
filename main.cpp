#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>  // for types

#include <csignal>
#include <filesystem>  // for std::filesystem
#include <iostream>    // for std::cerr
#include <thread>      // for std::thread

#include "cft/init.hpp"
#include "cft/parse.hpp"
#include "cft/tester.hpp"

pid_t server_id;
void serve() {
  std::string server_path =
      std::filesystem::canonical("/proc/self/exe").parent_path().string() +
      "/server";

  std::fstream serverLogs{"server.logs", std::ios::out};
  if (!serverLogs) {
    std::cerr << "server failed: failed to create logs\n";
    return;
  }

  char* argv[] = {server_path.data(), (char*)">/dev/null", (char*)"2>&1",
                  (char*)"&", nullptr};
  if (posix_spawn(&server_id, (char*)server_path.data(), NULL, NULL, argv,
                  NULL) != 0) {
    perror("server: server failed");
    return;
  }

  int wstatus;
  if (waitpid(server_id, &wstatus, WNOHANG) < 0) {
    perror("server: wait failed");
  }
  std::cerr << "server is running\n";
}

void endserver(int sig_id) {
  std::cerr << "server killed\n";
  kill(server_id, SIGKILL);
}

int main(int argc, char* argv[]) {
  serve();
  signal(SIGINT, endserver);
  std::string curdir = std::filesystem::current_path().string();

  Parse inputTokens(argc, argv);

  switch (inputTokens.whichAction()) {
    case query::JUDGE: {
      Tester cftester(argc, argv, curdir);

      auto disp = DISP_LOAD;
      auto frames = FR_BOXX;
      std::thread showAnim{spinnerBool, std::ref(disp), std::ref(frames),
                           std::ref(cftester.get_loaded_ref())};
      std::optional<status> failedrun = cftester.load_bin();
      showAnim.join();

      if (!failedrun) {
        cftester.judge();
        return 0;
      }

      switch (failedrun.value()) {
        case status::PROCESSING_ERR:
          std::cerr << BLACK_ON_RED << "Processing Error" << COLOR_END << '\n';
          std::cerr << "failed to run inputted file.\n";
          return 1;
        case status::RUNTIME_ERR:
          std::cout << BLACK_ON_RED << "Runtime Error" << COLOR_END << '\n';
          std::cerr << "Inputted file exited abnomrally, tests terminated.\n";
          return 1;
        case status::IDLENESS:
          std::cerr << BLACK_ON_RED << "Idleness time limit exceeded!"
                    << COLOR_END << '\n';
          std::cerr << "process did not any produce output for "
                    << IDLE_LIMIT.count() << " seconds \n";
          std::cerr << "\t  check for infnite loops or foregetting to output "
                       "to stdout! \n";
          return 1;
        default:
          return 0;
      }
    } break;
    case query::INIT: {
      Init create(argc, argv);
      create.createFiles();
    } break;
    default:
      return 0;
  }
}
