#pragma once

#include <cassert>
#include <cstdlib>
#include <string>

#include "../util/include.hpp"

class Init {
 private:
  int files = 1;
  int status = 0;

  const std::string curdir;
  std::string cleanfs;
  std::string templ;

 public:
  Init(int argc, char *argv[]);

  int createFiles();

  int configTempl();
};
