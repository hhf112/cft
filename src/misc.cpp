
#define utch reinterpret_cast<const char*>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

#include "cft/colors.hpp"
#include "cft/util.hpp"

std::string lowerCase(std::string& s) {
  std::string lower{};
  std::transform(s.begin(), s.end(), std::back_inserter(lower),
                 [](char c) { return std::tolower(c); });
  return lower;
}

void spinnerBool(std::array<std::string, 3>& display,
                 std::vector<std::string>& frames, int& done) {
  int frm = frames.size();
  int i = 0;
  while (!done) {
    std::cerr << BRIGHT_YELLOW_FG << display[0] << frames[i % frm]
              << '\r';
    i++;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if (done > 0)
    std::cerr << GREEN_FG << display[1];
  else
    std::cerr << RED_FG << display[2];
}
