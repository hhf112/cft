#include <bits/stdc++.h>
using namespace std;
using LL = long long int;

#define N 100000
#define mod 10000000
#define pb push_back
#define eb emplace_back
#define pii pair <int, int>
#define pll pair <LL, LL>
#define f first
#define s second

template <typename T>
T gcd (T a, T b) {
    if (b == 0) return a;
    return gcd (b, a % b);
}



inline void solve() {

}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
#endif

  ios::sync_with_stdio(false);
  cin.tie(0); cout.tie(0);

  int nt;  cin >> nt;
  while (nt--) {
    solve();
  }

  return 0;
}
