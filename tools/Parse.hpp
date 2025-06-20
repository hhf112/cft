#pragma once

#include <algorithm>  // fro std::transform
#include <iterator>   // std::back_inserter
#include <optional>   // for std::optional
#include <string>     // for std::string

#include "../util/include.hpp"

class Parse {
 private:
  query action;
  std::optional<query> m_decode(std::string cs, int argno);
  std::string m_lowerCase(std::string& s) {
    std::string lower{};
    std::transform(s.begin(), s.end(), std::back_inserter(lower),
                   [](char c) { return std::tolower(c); });
    return lower;
  }

 public:
  Parse(int argc, char* argv[]);
  query whichAction() { return action; }
};
