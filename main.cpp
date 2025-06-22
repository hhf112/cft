#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>  // for types

#include <cstdlib>
#include <filesystem>  // for std::filesystem
#include <iostream>    // for std::cerr
#include <thread>      // for std::thread

#include "cft/init.hpp"
#include "cft/parse.hpp"
#include "cft/tester.hpp"

void serve() {
  std::string run_cmd =
      std::filesystem::canonical("/proc/self/exe").parent_path().string() +
      "/cpcmp";
  std::cerr << "starting server...\n";

  pid_t server_id;
  char* args[] = {
      (char*)run_cmd.c_str(),
      (char*)">dev/null",
      (char*)"2>&1",
      (char*)"&",
      nullptr,
  };

  if (posix_spawn(&server_id, args[0], NULL, NULL, args, NULL) < 0) {
    perror("posix_spawn failed");
    return;
  }

  pid_t wstatus;
  if (waitpid(server_id, &wstatus, 0) < 0) {
    perror("wait on server failed");
    return;
  }
}

int main(int argc, char* argv[]) {
  serve();
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
