#pragma once

#include <signal.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <thread>

#include "../ansi_macros.hpp"
#include "../enums.hpp"

class Tester {
 private:
  int lpTestcase = 1;  // Lines per testcase.
  std::optional<int> runtime;
  int cnt = 0;
  int failcnt = 0;
  int testcnt = 0;

  std::string filename;

  status result = status::UNKNOWN;
  warning timeLimit = warning::GOOD;

  bool testCompleteOne() { return ++cnt % lpTestcase != 0; }

  // Possible unkonwn result, only to be used after tests have been run.
  status resultMU() { return result; }

 public:
  Tester(int argc, char* argv[], const std::string& curdir) {
    filename = curdir + "/" + argv[1];
    lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);
  }

  inline std::optional<status> loadBin() {
    // runtime
    pid_t binID;
    char* args[] = {filename.data(), nullptr};
    if (posix_spawn(&binID, filename.c_str(), NULL, NULL, args, NULL) != 0) {
      perror("posix failed");
      return status::PROCESSING_ERR;
    }
    auto start = std::chrono::high_resolution_clock::now();

    int binStatus;
    if (waitpid(binID, &binStatus, 0) < 0) {
      perror("wait failed");
      return status::PROCESSING_ERR;
    }
    auto end = std::chrono::high_resolution_clock::now();

    if (WIFEXITED(binStatus)) {
      return status::RUNTIME_ERR;
    }

    runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

    if (runtime >= 90) timeLimit = warning::TLE;

    return {};
  }

  inline void judge() {
    std::ofstream report{filename + "/report.txt", std::ios::out};
    if (!runTests(report)) result = status::PROCESSING_ERR;

    // FINAL VERDICT
    report << "FINAL VERDICT: \n";

    switch (result) {
      case status::AC:
        std::cout << WHITE_ON_GREEN << "Accepted\n" << COLOR_END;
        report << "All tests passed!\n";
        std::cout << "verdict: All tests passed.\n";
        break;

      case status::WA:
        std::cout << WHITE_ON_RED << "Wrong Answer\n" << COLOR_END;
        report << "WRONG ANSWER\n";
        std::cout << "verdict: test failed.\n";
        std::cout << failcnt << " test(s) failed\n";
        break;

      case status::NILIO:
        std::cout << WHITE_ON_CYAN << "I/O empty.\n" << COLOR_END;
        break;

      case status::WRONG_OUTPUT:
        std::cout << BLACK_ON_WHITE << "WRONG OUTPUT\n" << COLOR_END;
        report << "verdict: Invalid handling of input, tests terminated.\n";
        std::cout
            << "verdict: output length doesn't match expected length, test "
               "terminated.\n";
        break;

      case status::PROCESSING_ERR:
        std::cout << WHITE_ON_CYAN << "Processing Error\n" << COLOR_END;
        std::cout << "Unable to fetch required files.\n";

      default:
        //
    }

    if (!runtime) {
      std::cerr << "Runtime not evaluated due to invalid binary run. \n";
      return;
    }

    report << "runtime: " << runtime.value() << " ms.\n";
    std::cout << "runtime: " << runtime.value() << " ms.\n";

    // remmeber to color and add.
    switch (timeLimit) {
      case warning::TLE:
        std::cout << WHITE_ON_RED << "Warning: Possible TLE! :|\n" << COLOR_END;
        break;
      case warning::GOOD:
        std::cout << GREEN_FG << "runtime seems fine for a submit! :)\n"
                  << COLOR_END;
        break;
      default:
        //
    }

    report.close();
  }

  inline std::optional<status> runTests(std::ofstream& report) {
    if (!report) return {};
    // files
    std::ifstream output{filename + "/out.txt", std::ios::in};
    std::ifstream actualOutput{filename + "/output.txt", std::ios::in};

    if (!(output && actualOutput)) {
      return {};
    }

    if (output.eof()) {
      return result = status::NILIO;
    }

    std::string compare;
    std::string actual;
    std::string buf;

    std::cerr << BRIGHT_YELLOW_FG << "Judging...\n" << COLOR_END;
    while (std::getline(output, buf)) {
      compare += buf + '\n';
      if (!std::getline(actualOutput, buf)) {
        return result = status::WRONG_OUTPUT;
      }
      actual += buf + '\n';

      if (testCompleteOne()) {
        ++testcnt;
        report << compare;
        report << "->\n" << actual << '\n';

        std::cout << "test " << testcnt << ": ";
        if (compare != actual) {
          result = status::WA;
          failcnt++;
          report << "FAILED\n\n";
          std::cerr << RED_FG << "failed\n" << COLOR_END;
        } else {
          std::cerr << GREEN_FG << "passed\n" << COLOR_END;
        }
        compare.clear();
        actual.clear();
      }
    }

    if (!actualOutput.eof()) {
      return result = status::WRONG_OUTPUT;
    }

    output.close();
    actualOutput.close();

    if (result == status::UNKNOWN) return result = status::AC;

    return result;
  }
};
