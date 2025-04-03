#include "../tools/Tester.hpp"

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

Tester::Tester(int argc, char* argv[], std::string& cwd)
    : filename{std::move(cwd)} {
  filename += '/';
  filename += argv[1];
  lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);

  if (buildOn) {
    std::optional<buildErr> buildFail = build();
    if (!buildFail) {
      std::cerr << "\tbuild finsihed succesfully. \n";
      return;
    }

    switch (buildFail.value()) {
      case buildErr::PROCESSING_ERR:
        std::cerr << "Unable to fetch buildscripts. \n";
      case buildErr::NULL_BS:
        std::cerr << RED_FG << "No buildsrcipts found!\n" << COLOR_END << '\n';

      case buildErr::BUILD_FAIL:
        std::cerr << RED_FG << "Build failed\n" << COLOR_END << '\n';
      default:
        exit(1);
    }
  }
}
