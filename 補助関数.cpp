#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;

/*
使い方チートシート
乱数 (XorShift)rnd.next_int(10) : 0〜9のランダムな整数を生成します。
	rnd.next_int(5, 15) : 5〜15のランダムな整数を生成します。
	rnd.next_double() : 0.0〜1.0のランダムな小数を生成します（確率判定用）。

タイマー (FastTimer)timer.get_ms() : プログラム実行開始からの経過時間をミリ秒単位で取得します（例: if (timer.get_ms() > 1850) break;）。

高速指数関数 (FastExp)f_exp(delta / temp) : 進化計算の突然変異や焼きなまし法において、悪化遷移を許容する確率 $e^{\Delta/T}$ を事前計算テーブルから即座に返します。高負荷な std::exp の呼び出しを回避します。

世代管理配列 (RollbackArray)RollbackArray<int> visited(2500, 0); : 初期化。
	visited.set(index, 1) / visited.get(index) : 通常の配列のように読み書きします。	visited.clear() : 一瞬で全要素をリセットします。 memset やループによる初期化コス	トをゼロにします。
*/

// ==========================================
// 1. 高速乱数生成器 (XorShift)
// ==========================================
struct XorShift {
    uint32_t x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    inline uint32_t operator()() {
        uint32_t t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    inline int next_int(int max_val) { return (*this)() % max_val; }
    inline int next_int(int min_val, int max_val) { return min_val + (*this)() % (max_val - min_val + 1); }
    inline double next_double() { return (*this)() / 4294967296.0; }
} rnd;

// ==========================================
// 2. 高速タイマー
// ==========================================
struct FastTimer {
    chrono::high_resolution_clock::time_point start_time;
    FastTimer() { reset(); }
    inline void reset() { start_time = chrono::high_resolution_clock::now(); }
    inline double get_ms() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(now - start_time).count();
    }
} timer;

// ==========================================
// 3. 状態遷移用 高速指数関数 (テーブル参照)
// ==========================================
struct FastExp {
    vector<double> exp_table;
    const int table_size = 100000;
    const double range = 10.0; 
    
    FastExp() {
        exp_table.resize(table_size + 1);
        for (int i = 0; i <= table_size; ++i) {
            exp_table[i] = exp(-range * i / table_size);
        }
    }
    inline double operator()(double delta_over_temp) const {
        if (delta_over_temp >= 0) return 1.0;
        if (delta_over_temp <= -range) return 0.0;
        int idx = (int)(-delta_over_temp * table_size / range);
        return exp_table[idx];
    }
} f_exp;

// ==========================================
// 4. 世代管理配列 (O(1) 初期化)
// ==========================================
template <typename T>
struct RollbackArray {
    vector<T> data;
    vector<int> gen;
    int current_gen = 1;
    T default_val;

    RollbackArray(int size, T def_val = T()) : data(size, def_val), gen(size, 0), default_val(def_val) {}

    inline void clear() { current_gen++; } 

    inline T get(int idx) {
        if (gen[idx] != current_gen) return default_val;
        return data[idx];
    }

    inline void set(int idx, T val) {
        data[idx] = val;
        gen[idx] = current_gen;
    }
};