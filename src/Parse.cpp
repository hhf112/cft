#include <iostream>
#include <optional>
#include <string>

#include "../tools/Parse.hpp"

Parse::Parse(int argc, char* argv[]) {
  switch (argc) {
    case 2: {
      std::string arg(argv[1]);
      action = decode(arg, 1).value();
      break;
    }
    case 3: {
      std::string arg(argv[1]);
      std::string num(argv[2]);
      // User entered a filename with a numerical value.
      if (decode(arg, 1) != query::INIT) {
        std::cerr
            << "Invalid input. CFTester can only test 1 file at a time.\n";
        exit(1);
      }

      // user entered an invalid count of files to be created.
      if (decode(num, 2) != query::INIT) {
        std::cerr << "Invalid count of files to be generated.\n";
        std::cerr << "Possible count must lie within the positive range of a "
                     "positive 32 bit integer\n";
      }
      break;
    }
    default:
      std::cerr << BLACK_ON_WHITE
                << "Invalid number of arguments provided." COLOR_END << '\n';
      std::cerr << "Try:\n cft <filename> \n cft init \n "
                   "cft init 3 \n cft cl\n";
      exit(1);
  }
}
