#include <fstream>
#include <iostream>
#include <optional>

#include "../tools/Tester.hpp"

int Tester::judge() {
  std::ofstream report{"report.txt", std::ios::out};
  if (!runTests(report)) result = status::PROCESSING_ERR;

  // FINAL VERDICT
  report << "FINAL VERDICT: \n";

  std::cout << '\n';
  switch (result) {
    case status::AC:
      std::cout << WHITE_ON_GREEN << "Accepted" << COLOR_END << '\n';
      report << "All tests passed!\n";
      std::cout << "verdict: All tests passed.\n";
      break;

    case status::WA:
      std::cout << BLACK_ON_RED << "Wrong Answer" << COLOR_END << '\n';
      report << "WRONG ANSWER\n";
      std::cout << "verdict: test failed.\n";
      std::cout << failcnt << " test(s) failed\n";
      break;

    case status::NILIO:
      std::cout << WHITE_ON_CYAN << "I/O empty." << COLOR_END << '\n';
      std::cout << "verdict: input / output / expected output files are "
                   "empty. Nothing to test.\n";
      return 1;

    case status::WRONG_OUTPUT:
      std::cout << BLACK_ON_WHITE << "Wrong output." << COLOR_END << '\n';
      std::cout << "verdict: output length doesn't match expected length, test "
                   "terminated.\n";
      return 1;

    case status::PROCESSING_ERR:
      std::cout << WHITE_ON_CYAN << "Processing Error" << COLOR_END << '\n';
      std::cout << "Unable to fetch required files.\n";
      return 1;

    default:
      std::cout << BLACK_ON_WHITE << "Unknown error occured ..." << COLOR_END
                << '\n';
      return 1;
      //
  }

  if (!runtime) {
    std::cerr << "Runtime not evaluated due to invalid binary run. \n";
    return 1;
  }

  report << "runtime: " << runtime.value() << " ms.\n";
  std::cout << "runtime: " << runtime.value() << " ms.\n";

  switch (timeLimit) {
    case warning::TLE:
      std::cout << BLACK_ON_RED << "Warning: Possible TLE! :|" << COLOR_END
                << '\n';
      break;
    case warning::GOOD:
      std::cout << GREEN_FG << "runtime seems fine for a submit! :)"
                << COLOR_END << '\n';
      break;
    default:
      std::cout << BLACK_ON_WHITE << "Unknown error occured ... k:["
                << COLOR_END << '\n';
      return 1;
      //
  }

  return 0;
}
