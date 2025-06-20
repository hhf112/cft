#include <unistd.h>  // for types

#include <filesystem>  // for std::filesystem
#include <functional>  // std::functional
#include <iostream>    // for std::cerr
#include <thread>      // for std::thread

#include "./tools/include.hpp"
#include "./util/include.hpp"
#include "tools/Tester.hpp"

//
int main(int argc, char* argv[]) {
  std::string curdir = std::filesystem::current_path().string();

  Parse inputTokens(argc, argv);

  switch (inputTokens.whichAction()) {
    case query::JUDGE: {
      Tester cftester(argc, argv, curdir);

      auto disp = DISP_LOAD;
      auto frames = FR_BOXX;
      std::thread showAnim{spinnerBool, std::ref(disp), std::ref(frames),
                           std::ref(cftester.getLoadedRef())};
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
