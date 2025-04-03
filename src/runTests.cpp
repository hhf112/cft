#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"

std::optional<status> Tester::runTests(std::ofstream& report) {
  if (!report || !loaded) return {};
  // files
  std::ifstream output{"out.txt", std::ios::in};
  std::ifstream actualOutput{"output.txt", std::ios::in};

  if (!(output && actualOutput)) {
    return {};
  }

  if (output.eof()) {
    return result = status::NILIO;
  }

  std::string compare;
  std::string actual;
  std::string buf;

  std::cerr << BRIGHT_YELLOW_FG << "Judging..." << COLOR_END << '\n';

  while (std::getline(output, buf)) {
    compare += buf;
    compare += '\n';
    if (!std::getline(actualOutput, buf)) {
      return result = status::WRONG_OUTPUT;
    }
    actual += buf;
    actual += '\n';

    if (testCompleteOne()) {
      ++testcnt;
      report << "test " << testcnt << '\n';
      report << compare;
      report << "->\n" << actual << '\n';

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

    compare.clear();
    actual.clear();
  }

  if (!cnt) {
    return result = status::NILIO;
  }

  if (std::getline(actualOutput, buf)) {
    return result = status::WRONG_OUTPUT;
  }

  if (result == status::UNKNOWN) return result = status::AC;

  return result;
}
