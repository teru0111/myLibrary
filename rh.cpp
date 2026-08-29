#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

struct RollingHash {
    static const uint64_t mod = (1ULL << 61) - 1;
    using uint128_t = __uint128_t;
    vector<uint64_t> hash, power;

    // 構築 (O(N))
    RollingHash(const string &s, uint64_t base) {
        int n = s.size();
        hash.assign(n + 1, 0);
        power.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            hash[i + 1] = (uint64_t)((uint128_t)hash[i] * base % mod + s[i]) % mod;
            power[i + 1] = (uint64_t)((uint128_t)power[i] * base % mod);
        }
    }

    // [l, r) のハッシュ値を取得 (O(1))
    uint64_t get(int l, int r) const {
        uint64_t res = hash[r] + mod - (uint64_t)((uint128_t)hash[l] * power[r - l] % mod);
        if (res >= mod) res -= mod;
        return res;
    }

    // 2つのハッシュ値 h1 (長さ問わず) と h2 (長さ len2) を結合する (O(1))
    uint64_t connect(uint64_t h1, uint64_t h2, int len2) const {
        uint64_t res = (uint64_t)((uint128_t)h1 * power[len2] % mod) + h2;
        if (res >= mod) res -= mod;
        return res;
    }
};

// 【使用例】
int main() {
    string S = "abracadabra";
    
    // 衝突を防ぐため、基数(base)は実行ごとにランダム生成する
    random_device rd;
    mt19937_64 rng(rd());
    uint64_t base = rng() % (RollingHash::mod - 2) + 2;
    
    // 構築
    RollingHash rh(S, base);
    
    // "abra" の部分を比較 ( [0, 4) と [7, 11) )
    if (rh.get(0, 4) == rh.get(7, 11)) {
        cout << "Match!" << "\n";
    }
    
    return 0;
}
