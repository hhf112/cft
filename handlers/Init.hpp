#pragma once

#include <fstream>
#include <iostream>
#include <string>

class Init {
 private:
  int files = 1;
  std::string buf;
  std::string open;
  std::string cleanfs;
  const std::string curdir;

 public:
  Init(int argc, char* argv[], const std::string& cwd) : curdir {cwd} {
    // clear IO files.
    system("sh cl");

    {
      std::ofstream c1{curdir + "/input.txt", std::ios::out};
      std::ofstream c2{curdir + "/output.txt", std::ios::out};
      std::ofstream c4{curdir + "/out.txt", std::ios::out};
      std::ofstream c5{curdir + "/report.txt", std::ios::out};
    }

    // no of problem statements in the contest.
    files = (argc > 2 ? std::max(argv[2][0] - '0', 1) : 1);

    buf = R"( #include <bits/stdc++.h>
using LL = long long int;
using namespace std;
#define fast                   \
  ios::sync_with_stdio(false); \
  cin.tie(0);                  \
  cout.tie(0);

#define N 100000
#define inf INT_MAX
#define ninf INT_MIN
#define pb push_back()

inline void solve() {

}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
#endif

  fast int nt = 0;
  cin >> nt;
  while (nt--) {
    solve();
  }

  return 0;
})";

    cleanfs += "rm ";
    for (int i = 0; i < files; i++) {
      std::string num;
      num += curdir + "/" + char(i + 'a') + ".cpp";
      cleanfs += num + " " + char(i + 'a') + " ";

      std::ofstream f{num, std::ios::out};
      f << buf;
    }
  }

  bool queryCleanup() {
    std::ofstream clean{curdir + "cl", std::ios::out};
    if (!clean) {
      std::cerr << "Could not create cleaning logs\n";
      return 1;
    }
    clean << cleanfs;
    clean.close();

    return 0;
  }

  int updateBuffr() { return 0; }
};
