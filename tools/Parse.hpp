#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include <string>

#include "../util/include.hpp"

// oarse cli arguments,
class Parse {
 private:
  query action;

  // returns query based on a specific part of the argueent, orchestrated and
  // called in constructor.
  std::optional<query> m_decode(std::string cs, int argno);

  // used by decode.
  std::string m_lowerCase(std::string& s) {
    std::string lower{};
    std::transform(s.begin(), s.end(), std::back_inserter(lower),
                   [](char c) { return std::tolower(c); });
    return lower;
  }

 public:
  // sets action.
  Parse(int argc, char* argv[]);

  // self descriptive.
  query m_whichAction() { return action; }
};
