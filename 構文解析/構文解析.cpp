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

typedef string::const_iterator State; // 構文解析
class ParseError {}; // 構文解析
int expression(State &begin);
int term(State &begin);
int factor(State &begin);
int number(State &begin);
void consume(State &begin, char expected);

/*
方針
1. まず factor, number を埋める。
2. 次に、expressinとtermを埋める    
    (最初に一つ下の部分式を一つだけパースした後に、あと はループで回しながら、足したり引いたりする。)

[BNF]
expression
  = term (('+' | '-') term)*
term
  = factor (('*' | '/') factor)*
factor
  = number
  | '(' expression ')'
number
  = [0-9]+

[1+2*3の例]
expression
├─ term
│  └─ factor
│      └─ 1
├─ +
└─ term
   ├─ factor
   │   └─ 2
   ├─ *
   └─ factor
       └─ 3
*/

// 四則演算の式をパースして、その評価結果を返す。
int expression(State &begin) {
    int ret = term(begin);

    while(1) {
        if (*begin == '+') {
            begin++;
            ret += term(begin);
        } else if (*begin == '-') {
            begin++;
            ret -= term(begin);
        } else {
            break;
        }
    }

    return ret;
}

// 乗算除算の式をパースして、その評価結果を返す。
int term(State &begin) {
    int ret = factor(begin); // 【修正】number(begin) から factor(begin) に変更
    for (;;) {
        if (*begin == '*') {
            begin++;
            ret *= factor(begin); // 【修正】同上
        } else if (*begin == '/') {
            begin++;
            ret /= factor(begin); // 【修正】同上
        } else {
            break;
        }
    }
    return ret;
}

// 数字の列をパースして、その数を返す。
int number(State &begin) {
    int ret = 0;

    while (isdigit(*begin)) {
        ret *= 10;
        ret += *begin - '0';
        begin++;
    }

    return ret;
}

// 括弧か数をパースして、その評価結果を返す。
int factor(State &begin) {
    if (*begin == '(') {
        begin++; // '('を飛ばす。
        int ret = expression(begin);
        begin++; // ')'を飛ばす。
        
        // 下３行は、上３行のデバッグ用
        // consume(begin, '(');
        // int ret = expression(begin);
        // consume(begin, ')');

        return ret; // 【修正】計算結果を返す処理を追加
    } else {
        return number(begin);
    }
}

// beginがexpectedを指していたらbeginを一つ進める。
// デバッグ： 次に読み込む文字が期待するものでなかった場合はエラーを出力
void consume(State &begin, char expected) {
    if (*begin == expected) {
        begin++;
    } else {
        cerr << "Expected '" << expected << "' but got '" << *begin << "'"
            << endl;
        cerr << "Rest string is '";
        while (*begin) {
            cerr << *begin++;
        }
        cerr << "'" << endl;
        throw ParseError();
    }
}

int main(void) {
    string s;
    // 【修正】入力が終了(EOF)するまでループするように変更
    while (getline(cin, s)) {
        if (s.empty()) continue; // 空行対策
        
        // 【任意】入力に空白が含まれるとパースに失敗するため、空白を取り除く
        s.erase(remove(s.begin(), s.end(), ' '), s.end());

        State begin = s.begin();
        int ans = expression(begin);
        cout << ans << endl;
    }
    return 0;
}