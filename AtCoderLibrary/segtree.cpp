#ifndef ATCODER_SEGTREE_HPP
#define ATCODER_SEGTREE_HPP 1

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
    // n以上の最小の2の冪乗を返す
    unsigned int bit_ceil(unsigned int n) {
        unsigned int x = 1;
        while (x < n) x *= 2;
        return x;
    }
    // 立っている最下位ビットより下にある0の数を返す (gcc/clang用組み込み関数)
    int countr_zero(unsigned int n) {
        return __builtin_ctz(n);
    }
} // namespace internal
// ----------------------------------------------

#if __cplusplus >= 201703L

template <class S, auto op, auto e> struct segtree {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
                  "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
                  "e must work as S()");

#else

template <class S, S (*op)(S, S), S (*e)()> struct segtree {

#endif

  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S>& v) : _n(int(v.size())) {
        size = (int)internal::bit_ceil((unsigned int)(_n));
        log = internal::countr_zero((unsigned int)size);
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

}  // namespace atcoder

#endif  // ATCODER_SEGTREE_HPP

using namespace atcoder;

// ==========================================
// 動作確認用サンプル (Range Maximum Query)
// ==========================================

// セグメント木に乗せる型 S
using S = int;

// 区間に対する操作 (今回は最小値)
S op(S a, S b) {
    return max(a, b);
}

// 単位元 (最小値を取得する場合は十分に大きな値)
S e() {
    return 0; 
}

int main() {
    int n,q; cin >> n >> q;

    segtree<S, op, e> seg(n);
    for(int i=0;i<n;i++) seg.set(i,0);

    int l,r,pos,x;
    while(q--){
        int t; cin >> t;
        if(t==1){
            cin >> pos >> x;
            pos--;
            seg.set(pos,x);
        }else{ 
            cin >> l >> r;
            l--; r--;// 0-index 半開区間 [l,r) 添字に注意
            cout << seg.prod(l,r) << "\n";
        }
    }

    return 0;
}