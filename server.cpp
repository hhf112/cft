// #include <fstream>
// #include "nlohmann/json.hpp"
// #include "yhirose/httplib.h"
//
// int main() {
//   httplib::Server svr;
//
//   nlohmann::json j;
//   svr.Post("/", [&](const auto& req, auto& res) {
//         std::fstream inp {"input.txt", std::ios::out};
//         std::fstream outp {"out.txt", std::ios::out};
//
//         j = nlohmann::json::parse(req.body);
//         if (j["tests"].is_array()){
//             for (auto& x: j["tests"]) {
//                 inp << std::string(x["input"]);
//                 outp << std::string(x["output"]);
//             }
//         }
//   });
//
//   svr.listen("0.0.0.0", 27121);
//   return 0;
// }
//
//


#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
int main() {
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(27121);

  if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
    perror("failed to find to server");
    return 1;
  }

  listen(sock_fd, 1);

  char buffer[4096];
  int client_fd;
  while (true) {
    client_fd = accept(sock_fd, nullptr, nullptr);
    memset(buffer, 0, sizeof(buffer));
    read(client_fd, buffer, sizeof(buffer) - 1);
  }
  return 0;
}
