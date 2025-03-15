#include <filesystem> 
// ^ implied future windows adpation


#include <iostream>

#include "../enums.hpp"
#include "../handlers/Handlers.hpp"

int main(int argc, char* argv[]) {
  std::string curdir = std::filesystem::current_path();

  Parse inputTokens(argc, argv);

  switch (inputTokens.whichAction()) {
    case query::JUDGE:{
      Tester runBin (argc, argv, curdir);
      runBin.passVerdict();
    }
    case query::INIT: {
      Init create (argc, argv, curdir);
      create.queryCleanup();
    }
    case query::CONFIG: {

    }
    default:
      //
  }
}
