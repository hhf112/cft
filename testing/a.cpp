#include <bits/stdc++.h>
using namespace std;
using LL = long long int;
const LL mod = 1000000007;

template <typename T>
T gcd (T a, T b) {
	if (b == 0) return a;
	return gcd (b, a % b);
}

int powINT (int a, LL b) {
	if (b == 0) return 1;
	else if (b % 2 == 0) {
		int p =  powINT(a, b/2);
		return 1LL * p * p % mod;
	}
	else {
		int p = powINT(a, (b-1)/2);
		return (1LL * p * p % mod ) * a;
	}
}

#define eb emplace_back
#define pb push_back
#define f first
#define s second
#define all(x) x.begin(), x.end()
#define allL(x) a + 1, a + n + 1

#define pii pair <int, int>
#define vc vector 
#define vci vector <int>
#define vcl vector <LL>
#define mii unordered_map<int, int>
#define mll unordered_map<LL, LL>
#define pq priority_queue

#define FOR(i, n) for (int i = 0; i<n; i++) 
#define FORR(i, n) for (int i = 1; i<=n; i++) 
#define FR(x, v) for (auto& x: v)

const int N = 200000;
void solve() {

}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	int nt = 0; cin >> nt;
	while (nt--) {
		solve();
	}
	return 0;
}
