#pragma once
#include <optional>
#include <string>

#include "../util/include.hpp"

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
  Tester(int argc, char* argv[], std::string& cwd);

  std::optional<status> loadBin();

  std::optional<buildErr> build();

  int judge();

  std::optional<status> runTests(std::ofstream& report);
};
