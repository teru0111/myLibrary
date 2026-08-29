#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 扱うアイテム（AHC069のグループなどを想定）
struct Item {
    int id;
    int weight; // 消費リソース（人数、占有面積など）
    int value;  // 獲得スコア（基本支払額など）
};

// ========================================================================
// 優先度付きキューのための比較演算子 (Min-Heap化)
// C++のpqは最大値を取り出すため、ここを逆転させて
// 「最も価値が低い（＝最初に捨てるべき）アイテム」が top() に来るように設計します。
// ========================================================================
struct CompareItem {
    bool operator()(const Item& a, const Item& b) const {
        // 例: コスパ (value / weight) が高いものを残し、低いものを捨てたい場合
        // 割り算（浮動小数点演算）を避けるため、交差乗算で比較します
        return (long long)a.value * b.weight > (long long)b.value * a.weight;
    }
};

// ========================================================================
// 状態管理 (差分更新を徹底し、状態のディープコピーを防ぐ)
// ========================================================================
struct State {
    long long current_weight = 0;
    long long current_score = 0;
    // 現在採用しているアイテムの集合
    priority_queue<Item, vector<Item>, CompareItem> accepted_items;

    // アイテムを採用する処理 O(log K)
    inline void add_item(const Item& item) {
        current_weight += item.weight;
        current_score += item.value;
        accepted_items.push(item);
    }

    // アイテムを破棄（過去改変）する処理 O(log K)
    inline void remove_worst_item() {
        if (accepted_items.empty()) return;
        Item worst = accepted_items.top();
        accepted_items.pop();
        current_weight -= worst.weight;
        current_score -= worst.value;
    }
};

// ========================================================================
// 過去改変貪欲法のメインロジック O(N log K)
// ========================================================================
vector<Item> solve_retroactive_greedy(const vector<Item>& items, long long capacity) {
    State state;

    for (const auto& item : items) {
        // 1. まず無条件で採用してみる
        state.add_item(item);

        // 2. 容量オーバーなどの制約違反が発生した場合、
        //    制限を満たすまで「過去に選んだ中で一番コスパの悪いもの」を捨て続ける
        while (state.current_weight > capacity && !state.accepted_items.empty()) {
            state.remove_worst_item();
        }
    }

    // 最終的にキューに残っているものが、最適化された採用アイテム群
    vector<Item> result;
    result.reserve(state.accepted_items.size());
    while (!state.accepted_items.empty()) {
        result.push_back(state.accepted_items.top());
        state.accepted_items.pop();
    }
    return result;
}