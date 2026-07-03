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
*/

enum NodeType{
    VAL_T,
    VAL_F,
    VAR,
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_IMP, // ->
};

struct Node{
    NodeType type;
    char name;
    unique_ptr<Node> left;
    unique_ptr<Node> right;
};

unique_ptr<Node> parse(State begin){
    // 定数、変数
    if(*begin == 'T' || *begin == 'F' || (*begin >= 'a' && *begin <= 'k')){
        auto node = make_unique<Node>();
        if(*begin == 'T') node->type = VAL_T;
        else if(*begin == 'F') node->type = VAL_F;
        else{
            node->type = VAR; // それ以外なら変数である
            node->var_name = *begin;
        }
        begin++;
        return node;
    }

    // 否定
    if(*begin == '-'){
        bgein++;
        auto node = make_unique<Node>();
        node->type = OP_NOT;
        node->left = parse(begin); // ~(a&b) の a 側に飛ぶ
        return node;
    }
    
    // 括弧
    if(*begin == '('){
        begin++;
        auto left_child = parse(begin);

        NodeType op_type;
        if(*begin == '-'){
            op_type = OP_IMP;
            consume(begin,"->");
        }else{
            op_type = (op == '*')? OP_AND:OP_OR;
            begin++; 
        }

        auto right_child = parse(begin);
        begin++; // ')'

        auto node = make_unique<Node>(); // オブジェクト指向の書き方ですか？
        node->type = op_type;
        node->left = move(left_child);
        node->right = move(right_child);

        return node;
    }

    reutrn nullptr;
}

void consume(State &begin, string expected) {
    for(char c:expected){
        if(*begin != c) throw ParseError();
        ++begin;
    }
}

int main(void) {
    string s;
    while(1){
        cin >> s;
        if(s=="#") break;

        State begin = s.begin();
        auto root_left = parse(begin);
        consume(begin,"=");
        auto root_right = parse(begin);

        bool is_true = true;
        for(int bit=0;bit<(1<<('k'-'a'));bit++){
            bool left_val = evaluate(root_left.get(),bit);
            bool right_val = evaluate(root_right.get(),bit);

            if(left_val != right_val){
                is_true = false; 
                break;
            }
        }

        if(is_true) cout << "Yes\n"
    }
}