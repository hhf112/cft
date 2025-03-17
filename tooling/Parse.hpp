#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>

#include "../ansi_macros.hpp"
#include "../enums.hpp"

class Parse {
 private:
  query action;

  std::optional<query> decode(std::string& cs, int argno) {
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

  std::string lowerCase(std::string& s) {
    std::string lower{};
    std::transform(s.begin(), s.end(), std::back_inserter(lower),
                   [](char c) { return std::tolower(c); });
    return lower;
  }

 public:
  Parse(int argc, char* argv[]) {
    switch (argc) {
      case 2: {
        std::string arg(argv[1]);
        action = decode(arg, 1).value();
        break;
      }
      case 3: {
        std::string arg(argv[2]);
        // User entered a filename with a numerical value.
        if (decode(arg, 1) != query::INIT) {
          std::cerr
              << "Invalid input. CFTester can only test 1 file at a time.\n";
          exit(1);
        }

        // user entered an invalid count of files to be created.
        if (decode(arg, 2) != query::INIT) {
          std::cerr << "Invalid count of files to be generated.\n";
          std::cerr << "Possible count must lie within the positive range of a "
                       "positive 32 bit integer\n";
        }
        break;
      }
      default:
        std::cerr << BLACK_ON_WHITE
                  << "Invalid number of arguments provided.\n" COLOR_END;
        std::cerr << "Try: ./cftester <filename> \n ./cftester init \n "
                     "./cftester init 3 \n ./cftester cl\n";
        exit(1);
    }
  }

  inline query whichAction() { return action; }
};
