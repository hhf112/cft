#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include <string>

#include "../util/include.hpp"

class Parse {
 private:
  query action;

  std::optional<query> decode(std::string& cs, int argno);

  std::string lowerCase(std::string& s) {
    std::string lower{};
    std::transform(s.begin(), s.end(), std::back_inserter(lower),
                   [](char c) { return std::tolower(c); });
    return lower;
  }

 public:
  Parse(int argc, char* argv[]);

  query whichAction() { return action; }
};
