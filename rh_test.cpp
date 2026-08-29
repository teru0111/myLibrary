#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

// ※ここに前回の RollingHash 構造体が入ります

// 2つの部分文字列の LCP (最長共通接頭辞) の長さを求める
// rh1 の [l1, r1) と rh2 の [l2, r2) を比較
int getLCP(const RollingHash& rh1, int l1, int r1, const RollingHash& rh2, int l2, int r2) {
    // ok: 確実に一致する長さ, ng: 確実に不一致になる長さ
    int ok = 0;
    int ng = min(r1 - l1, r2 - l2) + 1;
    
    while (ng - ok > 1) {
        int mid = ok + (ng - ok) / 2;
        // mid 文字のハッシュ値が一致するか判定
        if (rh1.get(l1, l1 + mid) == rh2.get(l2, l2 + mid)) {
            ok = mid; // 一致したので、もっと長い接頭辞があるかもしれない
        } else {
            ng = mid; // 不一致なので、これ以上長い接頭辞はあり得ない
        }
    }
    return ok;
}

int main() {
    string S = "abracadabra";
    string T = "abraczxcv";
    
    random_device rd;
    mt19937_64 rng(rd());
    uint64_t base = rng() % ((1ULL << 61) - 3) + 2;
    
    RollingHash rhS(S, base);
    RollingHash rhT(T, base);
    
    // 1. 異なる文字列同士の比較
    int lcp_len = getLCP(rhS, 0, S.size(), rhT, 0, T.size());
    cout << "LCP Length: " << lcp_len << "\n"; // 5 ("abrac" が一致)
    
    // 2. 同じ文字列内の部分文字列同士の比較
    int lcp_inner = getLCP(rhS, 0, 4, rhS, 7, 11);
    cout << "Inner LCP: " << lcp_inner << "\n"; // 4 ("abra" と "abra" が一致)
    
    return 0;
}
