#include <bits/stdc++.h>
using namespace std;

//using.
using LL = long long int;
using mii = unordered_map<int, int>;
using pii = pair<int, int>;
using pll = pair<LL, LL>;
using ust = unordered_set<int>;
using pq = priority_queue<int>;
using pqg = priority_queue<int, vector<int>, greater<int>>;
using ms = multiset<int>;
using msg = multiset<int, greater<int>>;
using st = stack<int>;

#define pb push_back
#define ev emplace_back
#define f first
#define s second
#define all(x) x.begin(), x.end()
#define FOR(i, s, e) for (int i = (s); i < (e); i++)
#define FR(x, v) for (auto &x : v)

//debugging.
#define debug(x)                                                               \
  cerr << "{-}: ";                                                             \
  dbg(x);                                                                      \
  cerr << '\n';

template <typename T> void dbg(T cmp) { cerr << cmp << ','; }

void dbg(bool b) { cerr << (b ? "true," : "false,"); }

template <typename T> void dbg(T *a, int s, int e) {
  cerr << "[";
  FOR(i, s, e) dbg(a[i]);
  cerr << "]\n";
}

template <typename T, typename U> void dbg(pair<T, U> p) {
  cerr << '<' << p.f << ',' << p.s << ">,";
}

template <typename T> void dbg(vector<T> &v) {
  cerr << "[";
  FR(x, v) dbg(x);
  cerr << "]\n";
}

template <typename T> void dbg(multiset<T> &ms) {
  cerr << "[";
  FR(x, ms) dbg(x);
  cerr << "]\n";
}

template <typename T, typename U> void dbg(unordered_map<T, U> &mp) {
  cerr << "[";
  FR(x, mp) dbg(x);
  cerr << "]\n";
}

template <typename T> void dbg(unordered_set<T> &st) {
  cerr << "[";
  FR(x, st) dbg(x);
  cerr << "]\n";
}

//const.
const LL mod = 1000000007;
const int inf = INT_MAX;
const int ninf = INT_MIN;

//misc.
int powINT(int a, int b) {
  if (b == 0)
    return 1;
  int pw;
  if (b % 2 == 0) {
    pw = powINT(a, b / 2);
    return 1LL * pw * pw % mod;
  } else {
    pw = powINT(a, (b - 1) / 2);
    return (1LL * pw * pw % mod) * a;
  }
}

template <typename T> T gcd(T a, T b) {
  if (b == 0)
    return a;
  return gcd(a, a % b);
}

const int N = 200000;


void solve() {
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int nt;
  cin >> nt;
  while (nt--) {
    solve();
  }

  return 0;
}
