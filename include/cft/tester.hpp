#pragma once
#include <optional>  // for std::optional
#include <string>    // for std::string

#include "cft/enums.hpp"
#include "cft/config.hpp"

#define TIME_LIMIT std::chrono::milliseconds(50)
#define IDLE_LIMIT std::chrono::seconds(4)
#define POLLING_RATE std::chrono::milliseconds(15)

class Tester {
 private:
  std::optional<std::string> m_ship = "/mnt/d/";
  std::string m_filename;
  int m_lines_ptc = 1;
  int m_loaded = 0;
  int m_failcnt = 0;
  std::optional<int> m_runtime;
  bool m_isbuild = 1;

  status m_result = status::UNKNOWN;
  warning m_timeLimit = warning::GOOD;

  std::optional<status> m_run_bin(int& wstatus);

 public:
  Tester(int argc, char* argv[], std::string& cwd);
  int& get_loaded_ref() { return m_loaded; }
  status get_result() { return m_result; }
  int judge();
  std::optional<buildErr> build();
  std::optional<status> load_bin();
  std::optional<status> runTests(std::ofstream& report);
};
