#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include "colors.hpp"

#define utch reinterpret_cast<const char*>
#define BRIGHT_YELLOW_FG "\033[93m"


void anim(bool& done) {
  const char* frames[] = {"|", "/", "_", utch(u8"😖"), utch(u8"💥")};

  int i = 0;
  std::cerr << "   " << BRIGHT_YELLOW_FG << "building ..." << COLOR_END << '\r';
  while (!done) {
    std::cerr << std::left << std::setw(2) << frames[i % 5] << '\r';
    i++;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  std::cerr << GREEN_FG << utch(u8"✅ build completed successfully!")
            << COLOR_END << '\n';
}
int main() {
  bool done = false;

  auto strt = std::chrono::high_resolution_clock::now();
  std::thread start(anim, std::ref(done));

  while (std::chrono::high_resolution_clock::now() - strt <
         std::chrono::seconds(5)) {
    done = 0;
  }

  done =1;
    start.join();
  return 0;
}
