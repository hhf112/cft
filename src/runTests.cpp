#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "../tools/Tester.hpp"
#include "../util/include.hpp"

std::optional<status> Tester::runTests(std::ofstream& report) {
  if (!report || !m_loaded) return {};
  FileIterator output{"out.txt"};
  FileIterator actualOutput{"output.txt"};

  std::string compare;
  std::string actual;

  std::cerr << BRIGHT_YELLOW_FG << "Judging...📜" << COLOR_END << '\n';

        
  while (output.fetchNext(compare)) {
    if (!actualOutput.fetchNext(actual)) {
      return m_result = status::WRONG_OUTPUT;
    }

    if (isFullTest()) {
      ++m_testcnt;
      report << "test " << m_testcnt << '\n';
      report << compare << '\n';
      report << "->\n" << actual << "\n\n";

      std::cerr << "test " << m_testcnt << ": ";
      if (compare != actual) {
        m_result = status::WA;
        m_failcnt++;
        report << "FAILED\n\n";
        std::cerr << RED_FG << "failed" << COLOR_END << '\n';
      } else {
        std::cerr << GREEN_FG << "passed" << COLOR_END << '\n';
      }
    }
  }

  if (!m_cnt) {
    return m_result = status::NILIO;
  }

  if (actualOutput.fetchNext(actual)) {
    return m_result = status::WRONG_OUTPUT;
  }

  if (m_result == status::UNKNOWN) return m_result = status::AC;

  return m_result;
}
