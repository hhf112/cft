#pragma once
#include <optional>
#include <string>

#include "../util/include.hpp"

// Currently uses native linux libarires for spawning build scripts and binaries.
// Possible windows support for process spawning? but windows people don't use
// a CLI do they.
//                              ~ author has a windows laptop
//
//

// considering it runs pretty fast on system as compared on the judge ... this is wrong man I need
// to check this shit.
#define TIME_LIMIT 50

// building, running, judging.
class Tester {
 private:
  // Lines per testcase.
  int lpTestcase = 1;

  //null if binary not loaded
  std::optional<int> runtime;

  // Total number of lines in expected output.
  int cnt = 0;

  int failcnt = 0;
  int testcnt = 0;

  //binary name, will have no trailing extensions.
  std::string filename;

  status result = status::UNKNOWN;
  warning timeLimit = warning::GOOD;  // heh

  int loaded = 0;
  bool buildOn = 1;

  bool testCompleteOne() { return ++cnt % lpTestcase == 0; }


 public:
  // assigns filename then calls build if set.
  Tester(int argc, char* argv[], std::string& cwd);

  // currently spawns the generic cpp build script on the filename + ".cpp"
  // returns build status.
  // buildscripts support ? 
  std::optional<buildErr> build();

  // spawns built binary then records runtime.
  // possible error status handled in main.
  // currently doesn't detect idleness status.
  std::optional<status> loadBin();

  // calls runTests then passes verdict based on report.
  // exits 1 after handling possible errors.
  int judge();

  // Overwrites report.
  // possible error status returned is handled by judge.
  std::optional<status> runTests(std::ofstream& report);

  // Possible unkonwn result, try to use only after tests have been done.
  status resultMU() { return result; }
};
