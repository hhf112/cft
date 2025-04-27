#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

std::optional<status> Tester::runTests(std::ofstream& report) {
  if (!report || !loaded) return {};
  // files
  FileIterator output{"out.txt"};
  FileIterator actualOutput{"output.txt"};

  std::string compare;
  std::string actual;

  std::cerr << BRIGHT_YELLOW_FG << "Judging...📜" << COLOR_END << '\n';

        
  while (output.fetchNext(compare)) {
    if (!actualOutput.fetchNext(actual)) {
      return result = status::WRONG_OUTPUT;
    }

    if (doTest()) { /*cnt is incremented here*/
      ++testcnt;
      report << "test " << testcnt << '\n';
      report << compare << '\n';
      report << "->\n" << actual << "\n\n";

      std::cerr << "test " << testcnt << ": ";
      if (compare != actual) {
        result = status::WA;
        failcnt++;
        report << "FAILED\n\n";
        std::cerr << RED_FG << "failed" << COLOR_END << '\n';
      } else {
        std::cerr << GREEN_FG << "passed" << COLOR_END << '\n';
      }
    }
  }

  if (!cnt) {
    return result = status::NILIO;
  }

  if (actualOutput.fetchNext(actual)) {
    return result = status::WRONG_OUTPUT;
  }

  if (result == status::UNKNOWN) return result = status::AC;

  return result;
}
