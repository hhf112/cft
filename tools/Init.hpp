#pragma once

#include <cassert>
#include <cstdlib>
#include <string>

#include "../util/include.hpp"


//create files, template processing,
class Init {
 private:
  int files = 1;
  int status = 0;

  const std::string curdir;
  std::string cleanfs;
  std::string templ;

 public:
  //flushes IO files and loads the tepmplate path if a valid template is found.
  Init(int argc, char *argv[]);

  //Creates filea as specified by files and also initializes cleanfs.
  int createFiles();

  //Self descriptive.
  int configTempl();
};
