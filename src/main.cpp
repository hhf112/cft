#include <iostream>
#include <filesystem>
#include <iostream>

#include "../enums.hpp"
#include "../ansi_macros.hpp"

#include "../tooling/include.hpp"

int main(int argc, char* argv[]) {
  std::string curdir = std::filesystem::current_path();

  //All possible argument checks are done in Parser.hpp.
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
          std::cerr << WHITE_ON_RED << "Processing Error" << COLOR_END << '\n';
          std::cerr << "failed to run inputted file.\n";
          return 1;
        case status::RUNTIME_ERR:
          std::cout << WHITE_ON_RED << "Runtime Error" << COLOR_END << '\n';
          std::cerr
              << "Inputted file exited abnomrally, tests terminated.\n";
          return 1;
        default:
        return 0;
      }
    }
    break;
    case query::INIT: {
      Init create(argc, argv, curdir);
      create.queryCleanup();
    }
    break;
    // case query::CLEAN: {
    //
    // }

    default:
    return 0;
  }
}
