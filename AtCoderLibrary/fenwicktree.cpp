#ifndef ATCODER_FENWICKTREE_HPP
#define ATCODER_FENWICKTREE_HPP 1

#include <bits/stdc++.h>
using namespace std;
const long long INF = 1LL << 60; // 10^18 より大きい
const int inf = 1<<30; // 10^9 より大きい

typedef long long ll;
//a:97, A:65
const int dx[]={1,0,-1,0,-1,1,1,-1};
const int dy[]={0,1,0,-1,-1,-1,1,1};
const double PI = acos(-1);

// priority_queue<ll,vector<ll>,greater<ll>> q;
template<class T, class U> inline bool chmin(T& a, const U& b) { if (a > b) { a = b; return true; } return false; }
template<class T, class U> inline bool chmax(T& a, const U& b) { if (a < b) { a = b; return true; } return false; }


namespace atcoder {

// --- 依存関係を排除するための内部関数の実装 ---
namespace internal {
    // 符号付き整数型 T に対応する符号なし整数型を取得する
    template <class T>
    using to_unsigned_t = typename std::make_unsigned<T>::type;
}
// ----------------------------------------------

// Reference: https://en.wikipedia.org/wiki/Fenwick_tree
template <class T> struct fenwick_tree {
    using U = internal::to_unsigned_t<T>;

  public:
    fenwick_tree() : _n(0) {}
    explicit fenwick_tree(int n) : _n(n), data(n) {}

    // p番目(0-index)に x を加算する
    void add(int p, T x) {
        assert(0 <= p && p < _n);
        p++;
        while (p <= _n) {
            data[p - 1] += U(x);
            p += p & -p;
        }
    }

    // 区間 [l, r) の和を求める
    T sum(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        return sum(r) - sum(l);
    }

  private:
    int _n;
    std::vector<U> data;

    U sum(int r) {
        U s = 0;
        while (r > 0) {
            s += data[r - 1];
            r -= r & -r;
        }
        return s;
    }
};

}  // namespace atcoder

#endif  // ATCODER_FENWICKTREE_HPP

using namespace std;
using namespace atcoder;

// ==========================================
// 動作確認用サンプル (長さ n の数列 A の転倒数)
// 1 <= a[i] <= 100000 
// ==========================================
int main() {
    int n; cin >> n;
    fenwick_tree<ll> bit(101010);
    ll ans=0;
    for(int i=0;i<n;i++){
        int a; cin >> a;
        ans += bit.sum(a+1,100001); // a より大きい値の個数
        bit.add(a,1);
    }

    cout << ans << endl;
    return 0;
}
