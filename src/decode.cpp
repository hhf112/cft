
#include <optional>
#include <stdexcept>
#include <string>

#include "../tools/Parse.hpp"


std::optional<query> Parse::decode(std::string& cs, int argno) {
  switch (argno) {
    case 2: {
      // check for valid numerical.
      std::optional<int> validNum;
      try {
        validNum = std::stoi(cs);
      } catch (std::invalid_argument) {
        return {};
      }

      if (validNum.value() < 0) {
        return {};
      }

      return action = query::INIT;
    }

      // Pure string parsing.
    case 1: {
      std::string lcs(lowerCase(cs));

      if (lcs == "init") return action = query::INIT;
      if (lcs == "cl")
        return action = query::CLEAN;
      else
        return action = query::JUDGE;
    }
    default:
      return {};
  }
}
