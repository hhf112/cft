#include <filesystem>
#include <iostream>
#include "./tools/include.hpp"
#include "./util/include.hpp"
#include "tools/Tester.hpp"

int main(int argc, char* argv[]) {
  std::string curdir = std::filesystem::current_path().string();

  // All possible argument checks are done in Parser.hpp.
  Parse inputTokens(argc, argv);

  switch (inputTokens.whichAction()) {
    case query::JUDGE: {
      Tester cftester(argc, argv, curdir);
      std::optional<status> failedrun = cftester.loadBin();

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
                    std::cerr << "process did not any produce output for " << IDLE_LIMIT.count()<<  " seconds \n";
                    std::cerr << "\t  check for infnite loops or foregetting to output to stdout! \n";
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
