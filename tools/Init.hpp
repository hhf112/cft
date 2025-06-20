#pragma once
#include <string> // for std::string
//
#include "../util/include.hpp"

class Init {
 private:
  int m_files = 1;
  int m_status = 0;

  const std::string m_curdir;
  std::string m_cleanfs;
  std::string m_templ;

 public:
  Init(int argc, char *argv[]);
  int createFiles();
  int configTempl();
};
