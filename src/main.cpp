#include <filesystem>
// ^ implied future windows adpation

#include <iostream>

#include "../enums.hpp"
#include "../handlers/Handlers.hpp"

int main(int argc, char* argv[]) {
  std::string curdir = std::filesystem::current_path();

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
          std::cerr << WHITE_ON_RED << "Processing Error\n" << COLOR_END;
          std::cerr << "posix / wait ERR: Failed to run inputted binary as "
                       "child process.\n";
          return 1;
        case status::RUNTIME_ERR:
          std::cout << WHITE_ON_RED << "Runtime Error\n" << COLOR_END;
          std::cerr
              << "Inputted binary Exited abnomrally, tests cannot be done\n";
          return 1;
        default:
          //
      }
    }
    case query::INIT: {
      Init create(argc, argv, curdir);
      create.queryCleanup();
    }
    case query::CONFIG: {
    }
    default:
      //
  }
}
