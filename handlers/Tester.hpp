#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../enums.hpp"

class Tester {
 private:
  int lpTestcase = 1;
  int runtime = 0;
  int cnt = 0;
  int failcnt = 0;
  int testcnt = 0;

  std::string filename;

  status result = status::UNKNOWN;
  warning timeLimit = warning::GOOD;

  bool testCompleteOne() { return !(++cnt % lpTestcase); }

  void runTests() {
    // files
    std::ifstream output{filename + "/out.txt"};
    std::ifstream actualOutput{filename + "/output.txt"};
    std::ofstream report{filename + "/report.txt", std::ios::out};

    if (!(output && actualOutput && report)) {
      result = status::PROCESSING_ERR;
      return;
    }

    if (output.eof()) {
      result = status::NILIO;
      return;
    }

    std::string compare;
    std::string actual;
    std::string buf;

    std::cerr << "\033[93mJudging...\033[0m\n";  // 93 : Bright yellow
    while (std::getline(output, buf)) {
      compare += buf + '\n';
      if (!std::getline(actualOutput, buf)) {
        result = status::RUNTIME_ERR;
        return;
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
          std::cerr << "\033[31mfailed\033[0m\n";  // 31 : Bright red
        } else {
          std::cerr << "\033[92mpassed\033[0m\n";  // 92 : Bright green
        }
        compare = "";
        actual = "";
      }
    }

    output.close();
    actualOutput.close();

    if (result == status::UNKNOWN) result = status::AC;
  }

 public:
  Tester(int argc, char* argv[], const std::string& curdir) {
    filename = curdir + "/" + argv[1];
    lpTestcase = (argc > 2 ? std::max(std::stoi(argv[2]), 1) : 1);

    // runtime
    auto start = std::chrono::high_resolution_clock::now();

    pid_t testid = fork();
    if (testid < 0) {
      std::cerr<<"Failed to fork child process\n";
    }
    else if (testid == 0) {
      char* args[] = {filename.data(), nullptr};
      execvp(filename.data(), args);

      perror("Failed to spwan child proccess to run the main file\n");
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

    runTests();
  }

  void passVerdict() {
    std::ofstream report{filename + "/report.txt", std::ios::out};

    // FINAL VERDICT
    report << "FINAL VERDICT: \n";

    switch (result) {
      case status::AC:
        std::cerr << "\033[97;42mAccepted\033[0m\n";
        report << "All tests passed!\n";
        std::cout << "verdict: All tests passed.\n";
        break;

      case status::WA:
        std::cerr << "\033[97;41mWrong Answer\033[0m\n";
        report << "WRONG ANSWER\n";
        std::cout << "verdict: unexpected output.\n";
        std::cout << failcnt << " test(s) failed\n";
        break;

      case status::NILIO:
        std::cerr << "\033[97;46mI/O empty.\033[0m\n";  // 46 : cyan
        break;

      case status::RUNTIME_ERR:
        std::cerr << "\033[90;47mRuntime Error\033[0m\n";
        report << "Runtime error encountered, test terminated.\n";
        std::cout
            << "verdict: output length doesn't match expected length, test "
               "terminated.\n";
        break;

      case status::PROCESSING_ERR:
        std::cerr << "\033[90;47mProcessing Error\033[0m\n";
        std::cout << "Unable to fetch required files.\n";

      default:
        //
    }

    report << "runtime: " << runtime << " ms.\n";
    std::cout << "runtime: " << runtime << " ms.\n";

    // remmeber to color and add.
    switch (timeLimit) {
      case warning::TLE:
        std::cout << "watch out for TLE :o\n";
      case warning::GOOD:
        std::cout << "runtime seems fine for a submit! :)\n";
      default:
        //
    }

    report.close();
  }
};
