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
  int lineCount = 1;
  int runtime = 0;
  int cnt = 0;
  int failcnt = 0;
  int testcnt = 0;

  std::string filename;
  std::string compare;
  std::string actual;
  std::string buf;

  status result = status::UNKNOWN;

  void verdictStatement() {
    std::ofstream report{filename + "/report.txt", std::ios::out};

    // FINAL VERDICT
    report << "FINAL VERDICT: \n";

    switch (result) {
      case status::RUNTIME_ERR:
        std::cout << "\033[90;47mRuntime Error\033[0m\n";
        report << "Runtime error encountered, test terminated.\n";
        std::cout
            << "verdict: output length doesn't match expected length, test "
               "terminated.\n";
        break;

      case status::WA:
        std::cout << "\033[97;41mWrong Answer\033[0m\n";
        report << "WRONG ANSWER\n";
        std::cout << "verdict: unexpected output.\n";
        std::cout << failcnt << " test(s) failed\n";
        break;

      case status::NILIO:
        std::cout << "\033[97;46mI/O empty.\033[0m\n";  // 46 : cyan
        break;
      case status::AC:
        std::cout << "\033[97;42mAccepted\033[0m\n";
        report << "All tests passed!\n";
        std::cout << "verdict: All tests passed.\n";
        break;

      default:
        //
    }

    report << "runtime: " << runtime << " ms.\n";
    std::cout << "runtime: " << runtime << " ms.\n";

    report.close();
  }

  bool testCompleteOne() {
    return !(++cnt % lineCount);
  }

 public:
  Tester(int argc, char* argv[], const std::string& curdir) {
    filename = curdir + "/" + argv[1];
    lineCount = (argc > 2 ? std::max(argv[2][0] - '0', 1) : 1);

    // runtime
    // This is faster than fork execvp, tested. (legit 10 times)
    auto start = std::chrono::high_resolution_clock::now();
    system(filename.c_str());
    auto end = std::chrono::high_resolution_clock::now();
    runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
  }

  void judge() {
    // files
    std::ifstream output{filename + "/out.txt"};
    std::ifstream actualOutput{filename + "/output.txt"};
    std::ofstream report{filename + "/report.txt", std::ios::out};

    // loop parameters
    std::cerr << "\033[93mJudging...\033[0m\n";  // 93 : Bright yellow

    if (output.eof()) {
      result = status::NILIO;
      return;
    }

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

    verdictStatement();
  }
};
