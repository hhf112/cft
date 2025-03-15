#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../ansi_macros.hpp"
#include "../enums.hpp"

class Tester {
 private:
  int lpTestcase = 1;  // Lines per testcase.
  int runtime = 0;
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

    // runtime
    auto start = std::chrono::high_resolution_clock::now();

    pid_t testid = fork();
    if (testid < 0) {
      std::cerr << "Failed to run program as a child process\n";
      exit(1);
    } else if (testid == 0) {
      char* args[] = {filename.data(), nullptr};
      execvp(filename.data(), args);

      perror("Failed to spawn child proccess to run the main file");
      exit(1);
    } else {
      int status;
      if (wait(&status) < 0) {
        perror("Waiting on child proccess failed\n");
        exit(1);
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

    if (runtime > 1) timeLimit = warning::TLE;
  }

  void judge() {
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
        std::cout << "verdict: unexpected output.\n";
        std::cout << failcnt << " test(s) failed\n";
        break;

      case status::NILIO:
        std::cout << WHITE_ON_CYAN << "I/O empty.\n" << COLOR_END;  
        break;

      case status::RUNTIME_ERR:
        std::cout << BLACK_ON_WHITE << "Runtime Error\n" << COLOR_END;
        report << "Runtime error encountered, test terminated.\n";
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

    report << "runtime: " << runtime << " ms.\n";
    std::cout << "runtime: " << runtime << " ms.\n";

    // remmeber to color and add.
    switch (timeLimit) {
      case warning::TLE:
        std::cout << WHITE_ON_RED << "Warning: Possible TLE! :|\n" << COLOR_END;
      case warning::GOOD:
        std::cout << GREEN_FG << "runtime seems fine for a submit! :)\n"
                  << COLOR_END;
      default:
        //
    }

    report.close();
  }

  std::optional<status> runTests(std::ofstream& report) {
    if (!report) return {};
    // files
    std::ifstream output{filename + "/out.txt", std::ios::out};
    std::ifstream actualOutput{filename + "/output.txt", std::ios::out};

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
        return result = status::RUNTIME_ERR;
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
          std::cerr << "\033[31mfailed\033[0m\n";
          std::cerr << RED_FG << "failed\n" << COLOR_END;
        } else {
          std::cerr << GREEN_FG << "passed\n"
                    << COLOR_END;  
        }
        compare = "";
        actual = "";
      }
    }

    output.close();
    actualOutput.close();

    if (result == status::UNKNOWN) return result = status::AC;

    return result;
  }
};
