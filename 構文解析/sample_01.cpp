// 途中経過
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

/*
方針
1. まず factor, number を埋める。
2. 次に、expressinとtermを埋める    
    (最初に一つ下の部分式を一つだけパースした後に、あと はループで回しながら、足したり引いたりする。)

[BNF]
expression
  = <number> | if_<variable>_then_<expression>_else_<expression>
variable
  = <letter>
  | <letter><variable>
letter
  = a | b | ... | z
number
  = 0 | 1 | ... | 9 ひとけた

  メモ
構文解析するだけの問題はでないということ。
構文解析しながら情報を集めていく



*/
typedef string::const_iterator State; // 構文解析
class ParseError {}; // 構文解析
int expression(State &begin);
int term(State &begin);
int factor(State &begin);
int number(State &begin);
void consume(State &begin, string expected);
string variable(State &begin);

const long long MOD = 998244353; // 問題の指定するMOD

struct Node{
    int type; // number なら 0, if ... なら 1
    int val; // numberの値
    int var_id; // 変数に頂点番号を割り振る
    int then_node;
    int else_node; 
};

vector<Node> nodes;
unordered_map<string,int> var_to_id;

// 四則演算の式をパースして、その評価結果を返す。
int expression(State &begin) {
    int ret;
    if(isdigit(*begin)){ // number 
        nodes.push_back({0,number(begin),-1,-1,-1});
        return nodes.size()-1; // 追加したノードの ID を返す。
    }else{ // if ...
        consume(begin,"if_");
        string v=variable(begin);
        
        consume(begin,"_then_");
        int then_id = expression(begin);
        
        consume(begin,"_else_");
        int else_id = expression(begin);

        int vid;
        if(var_to_id.count(v)) vid = var_to_id[v];
        else{
            vid = var_to_id.size(); // 新しいノードIDの取得は、これまでのノード数+1 (0-index)
            var_to_id[v]=vid;
        }

        nodes.push_back({1,-1,vid,then_id,else_id});
        return nodes.size()-1; // 変数のIDとノードのIDをわけて考える。(0-index)
        // ノードは expression を表す (number or if ...)
    }
}


// この問題では必ず一桁
int number(State &begin) {
    int ret = *begin - '0';
    begin++;
    return ret;
}

// 文字列を返す
string variable(State &begin) {
    string s="";
    while(*begin!='\0' && 'a'<=*begin && *begin<='z'){
        s+=*begin;
        begin++;
    }
    return s;
}

// if_, _else_, _then_ をconsumeする。
void consume(State &begin, string expected) {
    for(char c:expected){
        if(*begin != c) throw ParseError();
        ++begin;
    }
}

int main(void) {
    string s;
    // 【修正】入力が終了(EOF)するまでループするように変更
    while (getline(cin, s)) {
        if (s.empty()) continue; // 空行対策
        
        // 【任意】入力に空白が含まれるとパースに失敗するため、空白を取り除く
        s.erase(remove(s.begin(), s.end(), ' '), s.end());

        nodes.reserve(200000); // 十分大きな数であり、かつそこそこ早く収まる

        State begin = s.begin();
        int root = expression(begin);

        int m=var_to_id.size();
        vector<ll> pow2(m+1);
        vector<int> state_var(m,-1);
        pow2[0]=1;
        for(int i=1;i<=m;i++) pow2[i] = (pow2[i-1]*2)%MOD;

        ll ans=0;
        int fixed = 0; // そのノードに至までに真偽が確定している変数の数
        auto dfs=[&](auto dfs,int v)->void{
            if(nodes[v].type == 0){ // 葉の処理
                ans = (ans + nodes[v].val*pow2[m-fixed])%MOD;
                return;
            }

            int vid = nodes[v].var_id;
            int prev = state_var[vid];

            // then に進む
            if(prev == -1){
                state_var[vid]=1;
                fixed++;
                dfs(dfs,nodes[v].then_node);
                state_var[vid]=-1;
                fixed--; // 帰りがけにこの変数は解放
            }else if(prev == 1){ // 今見ているノードの祖先のどこかで既にこの変数の真偽が確定している場合
                dfs(dfs,nodes[v].then_node);
            }

            // else に進む
            if(prev == -1){
                state_var[vid]=0;
                fixed++;
                dfs(dfs,nodes[v].else_node);
                state_var[vid]=-1;
                fixed--; // 帰りがけにこの変数は解放
            }else if(prev == 0){ // 今見ているノードの祖先のどこかで既にこの変数の真偽が確定している場合
                dfs(dfs,nodes[v].else_node);
            }
        };
        
        dfs(dfs,root);
        cout << ans << endl;

        break; // 入力はひとつ
    }
    return 0;
}