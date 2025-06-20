#pragma once
#include <optional>
#include <string>

#include "../util/include.hpp"

// Currently uses native linux libarires for spawning build scripts and
// binaries. Possible windows support for process spawning? but windows people
// don't use a CLI do they.
//                              ~ author has a windows laptop
//
//

// considering it runs pretty fast on system as compared on the judge ... this
// is wrong man I need to check this shit.
#define TIME_LIMIT std::chrono::milliseconds(50)
#define IDLE_LIMIT std::chrono::seconds(4)
#define POLLING_RATE std::chrono::milliseconds(15)

// building, running, judging.
class Tester {
 private:
  // export the built file to desired path.
  std::optional<std::string> m_ship = "/mnt/d/";

  // Lines per testcase.
  int m_lpTestcase = 1;

  // null if binary not loaded
  std::optional<int> m_runtime;

  // Total number of lines in expected output.
  int m_cnt = 0;

  int m_failcnt = 0;
  int m_testcnt = 0;

  // binary name, will have no trailing extensions.
  std::string m_filename;

  status m_result = status::UNKNOWN;
  warning m_timeLimit = warning::GOOD;  // heh

  int m_loaded = 0;
  bool m_buildOn = 1;

  bool isFullTest() { return ++m_cnt % m_lpTestcase == 0; }
  std::optional<status> run_bin(int& wstatus);

 public:
  // assigns filename then calls build if set.
  Tester(int argc, char* argv[], std::string& cwd);

  int& getLoadedRef() { return m_loaded; }

  // currently spawns the generic cpp build script on the filename + ".cpp"
  // returns build status.
  // buildscripts support ?
  std::optional<buildErr> build();

  // spawns built binary then records runtime.
  // called by main.
  // currently doesn't detect idleness status.
  std::optional<status> load_bin();

  // calls runTests then passes verdict based on report.
  // exits 1 after handling possible errors.
  int judge();

  // Overwrites report.
  // called by judge.
  std::optional<status> runTests(std::ofstream& report);

  // Possible unkonwn result, try to use only after tests have been done.
  status resultMU() { return m_result; }
};
