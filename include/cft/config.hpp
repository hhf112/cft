#include <fstream>
#include <sstream>
#include <string>

#include "nlohmann/json.hpp"

class cfg {
 public:
  int TIME_LIMIT;  // milliseconds
  int IDLE_LIMIT;  // milliseconds
  int POLLING_RATE;
  std::string INPUT_FILENAME;
  std::string OUTPUT_FILENAME;
  std::string EXPECTED_OUTPUT_FILENAME;
  std::string REPORT_FILENAME;
  std::string SHIP_FILENAME;
  cfg() {
    std::fstream cfg_file = std::fstream{"config.json", std::ios::in};
    std::stringstream cfgs;
    cfgs << cfg_file.rdbuf();
    std::string cfg = cfgs.str();

    nlohmann::json j = nlohmann::json::parse(cfg);
    if (j["TIME_LIMIT"].is_number()) TIME_LIMIT = j["TIME_LIMIT"];
    if (j["IDLE_LIMIT"].is_number()) IDLE_LIMIT = j["IDLE_LIMIT"];
    if (j["POLLING_RATE"].is_number()) POLLING_RATE = j["POLLING_RATE"];
    if (j["INPUT_FILENAME"].is_string()) INPUT_FILENAME = j["INPUT_FILENAME"];
    if (j["OUTPUT_FILENAME"].is_string())
      OUTPUT_FILENAME = j["OUTPUT_FILENAME"];
    if (j["EXPECTED_OUTPUT_FILENAME"].is_string())
      EXPECTED_OUTPUT_FILENAME = j["EXPECTED_OUTPUT_FILENAME"];
    if (j["REPORT_FILENAME"].is_string())
      REPORT_FILENAME = j["REPORT_FILENAME"];
    if (j["SHIP_FILENAME"].is_string()) SHIP_FILENAME = j["SHIP_FILENAME"];
  }

 private:
  template <typename T>
  T parse(std::string& inputStream) {}
};
