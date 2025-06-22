#include <fstream>

#include "nlohmann/json.hpp"
#include "yhirose/httplib.h"

int main() {
  httplib::Server svr;
  svr.set_socket_options([](socket_t sock) {
    int off = 0;
#ifdef SO_REUSEPORT
    ::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &off, sizeof(off));
#endif
  });

  std::fstream logs{"logs.txt", std::ios::out};
  if (!logs) {
    std::cerr << "unable to open log file\n";
    return 1;
  }

  logs << "starting server on => PORT 27121\n";
  logs.flush();

  nlohmann::json j;
  svr.Post("/", [&](const auto& req, auto& res) {
    std::fstream inp{"in.txt", std::ios::out};
    std::fstream outp{"out.txt", std::ios::out};

    if (!inp || !outp) {
      logs << "unable to open input/output files\n";
      logs.flush();
      return;
    }

    j = nlohmann::json::parse(req.body);
    if (j["tests"].is_array()) {
      for (auto& x : j["tests"]) {
        inp << std::string(x["input"]);
        outp << std::string(x["output"]);
      }
    }
  });

  svr.listen("0.0.0.0", 27121);
  logs << "port already in use, enjoy :)\n";
  logs.flush();
  return 0;
}
