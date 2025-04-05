
#include <iomanip>
#define utch reinterpret_cast<const char*>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

#include "./colors.hpp"

std::string lowerCase(std::string& s) {
  std::string lower{};
  std::transform(s.begin(), s.end(), std::back_inserter(lower),
                 [](char c) { return std::tolower(c); });
  return lower;
}

void buildSpinner(int& done) {
  const char* frames[] = {"|", "/", "_", utch(u8"😖"), utch(u8"💥")};

  int i = 0;
  while (!done) {
    std::cerr << BRIGHT_YELLOW_FG << "building " << COLOR_END << frames[i % 5] << '\r';
    i++;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  if (done > 0)
    std::cerr << GREEN_FG << utch(u8"✅ build completed successfully!")
              << COLOR_END << '\n';
  else
    std::cerr << RED_FG << utch(u8"❌ build failed!") << COLOR_END << '\n';
}
