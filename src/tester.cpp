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
    : m_filename{std::move(cwd)} {
  m_filename += '/';
  m_filename += argv[1];
  m_lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);

  if (m_buildOn) {
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
        std::cerr << "tester: unable to fetch buildscripts. \n";
        exit(1);
      case buildErr::NULL_BS:
        std::cerr << RED_FG << "tester: no buildsrcipts found!\n" << COLOR_END << '\n';
        exit(1);

      case buildErr::BUILD_FAIL:
        std::cerr << RED_FG << "tester: build failed\n" << COLOR_END << '\n';
        exit(1);
    }
  }
}
