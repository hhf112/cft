#include <fstream>
#include "nlohmann/json.hpp"
#include "yhirose/httplib.h"

int main() {
  httplib::Server svr;

  nlohmann::json j;
  svr.Post("/", [&](const auto& req, auto& res) {
        std::fstream inp {"input.txt", std::ios::out};
        std::fstream outp {"out.txt", std::ios::out};

        j = nlohmann::json::parse(req.body);
        if (j["tests"].is_array()){
            for (auto& x: j["tests"]) {
                inp << std::string(x["input"]);
                outp << std::string(x["output"]);
            }
        }
  });

  svr.listen("0.0.0.0", 27121);
  return 0;
}

