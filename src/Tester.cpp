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
#include <thread>

#include "../util/include.hpp"

Tester::Tester(int argc, char* argv[], std::string& cwd)
    : filename{std::move(cwd)} {
  filename += '/';
  filename += argv[1];
  lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);

  if (buildOn) {
    int buildFinish = 0;
    auto disp = DISP_BUILD;
    auto frames = FR_EXPLODE;
    std::thread startBuild(spinnerBool, std::ref(disp), std::ref(frames),
                           std::ref(buildFinish));

    std::optional<buildErr> buildFail = build();
    if (!buildFail) {
      buildFinish = 1;
      startBuild.join();
      return;
    } else {
      buildFinish = -1;
    }
    startBuild.join();

    switch (buildFail.value()) {
      case buildErr::PROCESSING_ERR:
        std::cerr << "Unable to fetch buildscripts. \n";
        exit(1);
      case buildErr::NULL_BS:
        std::cerr << RED_FG << "No buildsrcipts found!\n" << COLOR_END << '\n';
        exit(1);

      case buildErr::BUILD_FAIL:
        std::cerr << RED_FG << "Build failed\n" << COLOR_END << '\n';
        exit(1);
    }
  }
}
