#include "../tools/Tester.hpp"

#include <spawn.h>      // for posix
#include <sys/types.h>  // for types
#include <sys/wait.h>   // for waitpid
#include <unistd.h>     // for types

#include <cstdio>    // for perror
#include <cstdlib>   // for exit()
#include <fstream>   // for fstream
#include <iostream>  // for std::cerr
#include <optional>  // for std::optional
#include <string>    // for std::string
#include <thread>    // for std::thread

#include "../util/include.hpp"

//
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
        std::cerr << RED_FG << "tester: no buildsrcipts found!\n"
                  << COLOR_END << '\n';
        exit(1);

      case buildErr::BUILD_FAIL:
        std::cerr << RED_FG << "tester: build failed\n" << COLOR_END << '\n';
        exit(1);
    }
  }
}

extern char** environ;
std::optional<buildErr> Tester::build() {
  std::string sourcefile = m_filename + ".cpp";
  char* args[] = {(char*)"g++", (char*)sourcefile.c_str(), (char*)"-o",
                  (char*)m_filename.c_str(), NULL};
  pid_t build_id;
  if (posix_spawnp(&build_id, "g++", NULL, NULL, args, environ) != 0) {
    perror("build: posix failed");
    return buildErr::PROCESSING_ERR;
  }
  int status;
  if (waitpid(build_id, &status, 0) < 0) {
    perror("build: wait failed");
    return buildErr::PROCESSING_ERR;
  }

  if (WIFEXITED(status))
    std::cerr << BRIGHT_YELLOW_FG
              << "build exit status: " << WEXITSTATUS(status) << COLOR_END
              << '\n';
  else {
    std::cerr << "build: unknown error occured\n" << '\n';
    return buildErr::PROCESSING_ERR;
  }

  if (m_ship) {
    std::fstream readsrc{sourcefile, std::ios::in};
    std::fstream cp{m_ship.value() + "ship.cpp", std::ios::out};

    if (!readsrc || !cp)
      std::cerr << "Unable to ship!\n";
    else {
      cp << readsrc.rdbuf();
      std::cerr << GREEN_FG << "\t shipped!\n";
    }
  }

  return {};
}

std::optional<status> Tester::runTests(std::ofstream& report) {
  if (!report || !m_loaded) return {};
  FileIterator output{"out.txt"};
  FileIterator actualOutput{"output.txt"};

  std::string compare;
  std::string actual;
  std::cerr << BRIGHT_YELLOW_FG << "Judging...📜" << COLOR_END << '\n';
  while (output.fetchNext(compare)) {
    if (!actualOutput.fetchNext(actual)) {
      return m_result = status::WRONG_OUTPUT;
    }
    if (isFullTest()) {
      ++m_testcnt;
      report << "test " << m_testcnt << '\n';
      report << compare << '\n';
      report << "->\n" << actual << "\n\n";
      std::cerr << "test " << m_testcnt << ": ";
      if (compare != actual) {
        m_result = status::WA;
        m_failcnt++;
        report << "FAILED\n\n";
        std::cerr << RED_FG << "failed" << COLOR_END << '\n';
      } else {
        std::cerr << GREEN_FG << "passed" << COLOR_END << '\n';
      }
    }
  }
  if (!m_cnt) {
    return m_result = status::NILIO;
  }
  if (actualOutput.fetchNext(actual)) {
    return m_result = status::WRONG_OUTPUT;
  }
  if (m_result == status::UNKNOWN) return m_result = status::AC;
  return m_result;
}
