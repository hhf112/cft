#include <iostream>
#include <sstream>

#include "./include/nlohmann/json.hpp"
#include "fstream"

using namespace nlohmann;
int main() {
  std::fstream cfg_file = std::fstream{"config.json", std::ios::in};
  std::stringstream cfgs;
  cfgs << cfg_file.rdbuf();

  json j = json::parse(cfgs.str());

  if (j["some number"].is_array()) {
    std::cout << typeid(j["some number"]).name() << '\n';
    std::vector<int> some = j["some number"];
    for (auto& x : some) std::cout << x << '\n';
  }

  return 0;
}
