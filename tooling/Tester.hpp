#pragma once

#include <errno.h>
#include <signal.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../ansi_macros.hpp"
#include "../enums.hpp"

class Tester {
 private:
  int lpTestcase = 1;  // Lines per testcase.
  std::optional<int> runtime;
  int cnt = 0;  // Total number of lines in expected output.
  int failcnt = 0;
  int testcnt = 0;

  std::string filename;

  status result = status::UNKNOWN;
  warning timeLimit = warning::GOOD;

  bool loaded = 0;
  bool buildOn = 1;

  bool testCompleteOne() { return ++cnt % lpTestcase == 0; }

  // Possible unkonwn result, only to be used after tests have been run.
  status resultMU() { return result; }

 public:
  Tester(int argc, char* argv[], const std::string& cwd) : filename{std::move(cwd)} {
    filename += '/' ;
    filename += argv[1];
    lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
  }

  inline std::optional<status> build() { return {};}

  inline std::optional<status> loadBin() {
    // runtime
    pid_t binID;
    char* args[] = {filename.data(), nullptr};
    if (posix_spawn(&binID, filename.data(), NULL, NULL, args, NULL) != 0) {
      perror("posix failed");
      return result = status::PROCESSING_ERR;
    }
    auto start = std::chrono::high_resolution_clock::now();

    int binStatus;
    if (waitpid(binID, &binStatus, 0) < 0) {
      // may or may not add detailed error cases here.
      perror("wait failed");
      return result = status::PROCESSING_ERR;
    }
    auto end = std::chrono::high_resolution_clock::now();

    if (!WIFEXITED(binStatus)) {
      if (WIFSIGNALED(binStatus)) {
        std::cerr << "child terminated by signal " << WTERMSIG(binStatus)
                  << '\n';
      }
      return status::RUNTIME_ERR;
    } else {
      std::cerr << "child exited succesfully with exit code "
                << WEXITSTATUS(binStatus) << '\n';
    }

    runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

    if (runtime >= 150) timeLimit = warning::TLE;

    loaded = 1;
    return {};
  }

  inline int judge() {
    std::ofstream report{"report.txt", std::ios::out};
    if (!runTests(report)) result = status::PROCESSING_ERR;

    // FINAL VERDICT
    report << "FINAL VERDICT: \n";

    std::cout << '\n';
    switch (result) {
      case status::AC:
        std::cout << WHITE_ON_GREEN << "Accepted" << COLOR_END << '\n';
        report << "All tests passed!\n";
        std::cout << "verdict: All tests passed.\n";
        break;

      case status::WA:
        std::cout << WHITE_ON_RED << "Wrong Answer" << COLOR_END << '\n';
        report << "WRONG ANSWER\n";
        std::cout << "verdict: test failed.\n";
        std::cout << failcnt << " test(s) failed\n";
        break;

      case status::NILIO:
        std::cout << WHITE_ON_CYAN << "I/O empty." << COLOR_END << '\n';
        return 1;

      case status::WRONG_OUTPUT:
        std::cout << BLACK_ON_WHITE << "Wrong output." << COLOR_END << '\n';
        std::cout
            << "verdict: output length doesn't match expected length, test "
               "terminated.\n";
        return 1;

      case status::PROCESSING_ERR:
        std::cout << WHITE_ON_CYAN << "Processing Error" << COLOR_END << '\n';
        std::cout << "Unable to fetch required files.\n";
        return 1;

      default:
        std::cout << BLACK_ON_WHITE << "Unknown error occured ..." << COLOR_END
                  << '\n';
        return 1;
        //
    }

    if (!runtime) {
      std::cerr << "Runtime not evaluated due to invalid binary run. \n";
      return 1;
    }

    report << "runtime: " << runtime.value() << " ms.\n";
    std::cout << "runtime: " << runtime.value() << " ms.\n";

    switch (timeLimit) {
      case warning::TLE:
        std::cout << WHITE_ON_RED << "Warning: Possible TLE! :|" << COLOR_END
                  << '\n';
        break;
      case warning::GOOD:
        std::cout << GREEN_FG << "runtime seems fine for a submit! :)"
                  << COLOR_END << '\n';
        break;
      default:
        std::cout << BLACK_ON_WHITE << "Unknown error occured ... k:["
                  << COLOR_END << '\n';
        return 1;
        //
    }

    return 0;
  }

  inline std::optional<status> runTests(std::ofstream& report) {
    if (!report || !loaded) return {};
    // files
    std::ifstream output{"out.txt", std::ios::in};
    std::ifstream actualOutput{"output.txt", std::ios::in};

    if (!(output && actualOutput)) {
      return {};
    }

    if (output.eof()) {
      return result = status::NILIO;
    }

    std::string compare;
    std::string actual;
    std::string buf;

    std::cerr << BRIGHT_YELLOW_FG << "Judging..." << COLOR_END << '\n';
    while (std::getline(output, buf)) {
      compare += buf;
      compare += '\n';
      if (!std::getline(actualOutput, buf)) {
        return result = status::WRONG_OUTPUT;
      }
      actual += buf;
      actual += '\n';

      if (testCompleteOne()) {
        ++testcnt;
        report << "test " << testcnt << '\n';
        report << compare;
        report << "->\n" << actual << '\n';

        std::cerr << "test " << testcnt << ": ";
        if (compare != actual) {
          result = status::WA;
          failcnt++;
          report << "FAILED\n\n";
          std::cerr << RED_FG << "failed" << COLOR_END << '\n';
        } else {
          std::cerr << GREEN_FG << "passed" << COLOR_END << '\n';
        }
      }

      compare.clear();
      actual.clear();
    }

    if (std::getline(actualOutput, buf)) {
      return result = status::WRONG_OUTPUT;
    }

    if (result == status::UNKNOWN) return result = status::AC;

    return result;
  }
};
