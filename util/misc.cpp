#include <algorithm>
#include <iterator>
#include <string>

std::string lowerCase(std::string& s) {
  std::string lower{};
  std::transform(s.begin(), s.end(), std::back_inserter(lower),
                 [](char c) { return std::tolower(c); });
  return lower;
}
