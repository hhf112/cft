#include "../tools/Parse.hpp"

#include <iostream>  // for std::cerr
#include <optional>  // for std::optional
#include <string>    // for std::string

Parse::Parse(int argc, char* argv[]) {
  switch (argc) {
    case 2: {
      action = decode(argv[1], 1).value();
      break;
    }
    case 3: {
      std::string arg(argv[1]);
      std::string num(argv[2]);
      if (decode(argv[1], 3) != query::INIT) {
        std::cerr << "parse: Invalid input. CFTester can only test 1 file at a "
                     "time.\n";
        exit(1);
      }

      if (decode(num, 2) != query::INIT) {
        std::cerr << "parse: Invalid count of files to be generated.\n\t";
        std::cerr << "Possible count must lie within the positive range of a "
                     "positive 32 bit integer\n";
      }
      break;
    }
    default:
      std::cerr << BLACK_ON_WHITE
                << "parse: Invalid number of arguments provided." COLOR_END
                << '\n';
      std::cerr << "Try:\n cft <filename> \n cft init \n "
                   "cft init 3 \n cft cl\n";
      exit(1);
  }
}

std::optional<query> Parse::decode(std::string cs, int argno) {
  switch (argno) {
    case 2: {
      std::optional<int> validNum;
      try {
        validNum = std::stoi(cs);
      } catch (std::invalid_argument) {
        return {};
      }

      if (validNum.value() < 0 || validNum > 52) {
        return {};
      }

      return action = query::INIT;
    }

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
