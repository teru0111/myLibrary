// 幾何学のテンプレまとめ
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

const double EPS = 1e-10;

// 幾何ライブラリの基本構造体と関数
struct Point {
    double x, y;
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
    Point operator/(double k) const { return {x / k, y / k};}
};
struct Circle {
    Point c;    // 中心
    double r;   // 半径
};
// ベクトルの基本演算
double dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
double cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
double norm(Point a) { return a.x * a.x + a.y * a.y; }
double abs(Point a) { return std::sqrt(norm(a)); }

// ==========================================
// 1. 射影 (Projection)
// 点 p1 と p2 を通る直線に対する、点 p の射影（垂線の足）を返す
// ==========================================
Point project(Point p1, Point p2, Point p) {
    Point v = p2 - p1;
    Point w = p - p1;
    double t = dot(w, v) / norm(v);
    return p1 + v * t;
}

// ==========================================
// 2. 反射 (Reflection)
// 点 p1 と p2 を通る直線を対称軸として、点 p と線対称な点を返す
// ==========================================
Point reflect(Point p1, Point p2, Point p) {
    // まず射影（垂線の足 x）を求める
    Point x = project(p1, p2, p);
    
    // 点 p から 点 x へのベクトル (x - p) を2倍して p に足す
    return p + (x - p) * 2.0; 
}

// ==========================================
// CCW (Counter Clock-Wise)
// 3点 p0, p1, p2 の位置関係を判定する
// ==========================================
int ccw(Point p0, Point p1, Point p2) {
    Point a = p1 - p0;
    Point b = p2 - p0;
    
    // 外積が正なら反時計回り
    if (cross(a, b) > EPS) return 1;    // COUNTER_CLOCKWISE
    // 外積が負なら時計回り
    if (cross(a, b) < -EPS) return -1;  // CLOCKWISE
    
    // 同一直線上にある場合 (外積がほぼ0)
    // ベクトルaとベクトルbが逆方向（内積が負） => p2 - p0 - p1 の順
    if (dot(a, b) < -EPS) return 2;     // ONLINE_BACK
    // ベクトルaよりベクトルbの方が長い => p0 - p1 - p2 の順
    if (norm(a) < norm(b)) return -2;   // ONLINE_FRONT
    
    // それ以外（p2が線分p0p1上にある）
    return 0;                           // ON_SEGMENT
}

// ==========================================
// 直線の平行・直交判定
// 平行なら 2、直交なら 1、それ以外なら 0 を返す
// ==========================================
int isOrthogonalOrParallel(Point p0, Point p1, Point p2, Point p3) {
    Point v1 = p1 - p0; // 直線 s1 の方向ベクトル
    Point v2 = p3 - p2; // 直線 s2 の方向ベクトル
    
    // 外積が0なら平行
    if (std::abs(cross(v1, v2)) < EPS) {
        return 2;
    }
    // 内積が0なら直交
    if (std::abs(dot(v1, v2)) < EPS) {
        return 1;
    }
    // それ以外
    return 0;
}

// ==========================================
// 線分の交差判定
// 交差するなら 1、しないなら 0 を返す
// ==========================================
int intersect(Point p0, Point p1, Point p2, Point p3) {
    // 条件1: 線分 p0-p1 に対して p2, p3 が異なる側にあるか
    bool cond1 = (ccw(p0, p1, p2) * ccw(p0, p1, p3) <= 0);
    
    // 条件2: 線分 p2-p3 に対して p0, p1 が異なる側にあるか
    bool cond2 = (ccw(p2, p3, p0) * ccw(p2, p3, p1) <= 0);
    
    // 両方の条件を満たせば交差している
    if (cond1 && cond2) {
        return 1;
    }
    return 0;
}
// ==========================================
// 2つの円の位置関係 (共通接線の数)
// 4: 離れている, 3: 外接, 2: 交わる, 1: 内接, 0: 内包
// ==========================================
int intersect_circle(Circle c1, Circle c2) {
    // 中心間の距離 d
    double d = abs(c1.c - c2.c);
    
    // EPS を用いて上から順に判定していく
    
    // 1. 離れている (d > r1 + r2)
    if (d > c1.r + c2.r + EPS) {
        return 4;
    }
    // 2. 外接する (d == r1 + r2)
    // 上の if を抜けている時点で d <= r1 + r2 + EPS が保証されているため、
    // r1 + r2 - EPS よりも大きければ実質的に等しいと判定できる
    if (d > c1.r + c2.r - EPS) {
        return 3;
    }
    // 3. 交わる (d > |r1 - r2|)
    if (d > std::abs(c1.r - c2.r) + EPS) {
        return 2;
    }
    // 4. 内接する (d == |r1 - r2|)
    if (d > std::abs(c1.r - c2.r) - EPS) {
        return 1;
    }
    // 5. 内包する (d < |r1 - r2|)
    return 0;
}

// ==========================================
// 線分の交点計算
// ※事前に intersect() で交差確認済みであることを前提とする
// ==========================================
Point getCrossPoint(Point p0, Point p1, Point p2, Point p3) {
    Point base = p3 - p2;
    
    // 直線 p2-p3 と点 p0, p1 が作る平行四辺形の面積（外積の絶対値）
    double d1 = std::abs(cross(base, p0 - p2));
    double d2 = std::abs(cross(base, p1 - p2));
    
    // 線分 p0-p1 を d1 : d2 に内分する比率 t
    double t = d1 / (d1 + d2);
    
    // p0 から p1 へ向かうベクトルを t 倍して p0 に足す
    return p0 + (p1 - p0) * t;
}

// ==========================================
// 点 p と線分 a-b の距離を求める関数
// ==========================================
double getDistanceLP(Point a, Point b, Point p) {
    // 垂線の足が端点aの外側にある場合（aからbへのベクトルと、aからpへのベクトルのなす角が鈍角）
    if (dot(b - a, p - a) < 0.0) return abs(p - a);
    // 垂線の足が端点bの外側にある場合（bからaへのベクトルと、bからpへのベクトルのなす角が鈍角）
    if (dot(a - b, p - b) < 0.0) return abs(p - b);
    // 垂線の足が線分上にある場合（直線との距離を外積で求める）
    return std::abs(cross(b - a, p - a)) / abs(b - a);
}

// ==========================================
// 線分 s1 (p0-p1) と線分 s2 (p2-p3) の距離を求める関数
// ==========================================
double getDistanceSS(Point p0, Point p1, Point p2, Point p3) {
    // 1. 交差している場合は距離 0
    if (intersect(p0, p1, p2, p3)) {
        return 0.0;
    }
    // 2. 交差していない場合は、4パターンの最小値
    return std::min({
        getDistanceLP(p2, p3, p0), // 点 p0 と 線分 p2-p3
        getDistanceLP(p2, p3, p1), // 点 p1 と 線分 p2-p3
        getDistanceLP(p0, p1, p2), // 点 p2 と 線分 p0-p1
        getDistanceLP(p0, p1, p3)  // 点 p3 と 線分 p0-p1
    });
}
// ==========================================
// 多角形の面積 (再掲)
// ==========================================
double getPolygonArea(const std::vector<Point>& poly) {
    double area = 0.0;
    int n = poly.size();
    if (n < 3) return 0.0;
    for (int i = 0; i < n; ++i) {
        area += cross(poly[i], poly[(i + 1) % n]);
    }
    return std::abs(area) / 2.0;
}

// ==========================================
// 多角形の点の包含判定 (Point-in-Polygon)
// 内部: 2, 辺上: 1, 外部: 0
// ==========================================
int contains(const std::vector<Point>& poly, Point p) {
    bool in = false; // 内部にいるかどうかのフラグ
    int n = poly.size();
    
    for (int i = 0; i < n; ++i) {
        // 点 p を原点 (0, 0) に平行移動して考える
        Point a = poly[i] - p;
        Point b = poly[(i + 1) % n] - p;
        
        // 1. 辺上判定
        // 外積が0 (一直線上) かつ 内積が0以下 (pを挟んで逆方向、またはpと一致)
        if (std::abs(cross(a, b)) < EPS && dot(a, b) < EPS) {
            return 1; // 辺上
        }
        
        // 2. レイキャスト（半直線との交差判定）
        // 常に a の Y座標 <= b の Y座標 となるように揃える（下から上へ向かうベクトルにする）
        if (a.y > b.y) std::swap(a, b);
        
        // a の Y座標が 0 以下、かつ b の Y座標が 0 より大きい（X軸を確実に跨ぐ）
        // かつ、外積が正（交点が原点 p より右側にある）
        if (a.y < EPS && EPS < b.y && cross(a, b) > EPS) {
            in = !in; // 交差するたびにフラグを反転
        }
    }
    
    return in ? 2 : 0;
}

// ==========================================
// 多角形の凸性判定
// 凸多角形なら 1、そうでない(凹多角形)なら 0 を返す
// ※頂点が時計回り・反時計回りのどちらで与えられても機能します
// ==========================================
int isConvex(const vector<Point>& poly) {
    int n = poly.size();
    if (n < 3) return 0;
    
    int sign = 0;
    for (int i = 0; i < n; ++i) {
        Point a = poly[(i + 1) % n] - poly[i];
        Point b = poly[(i + 2) % n] - poly[(i + 1) % n];
        double c = cross(a, b);
        
        if (c > EPS) {
            // 反時計回りの折れ曲がりが発生
            if (sign == -1) return 0; // 既に時計回りがあったら凹
            sign = 1;
        } else if (c < -EPS) {
            // 時計回りの折れ曲がりが発生
            if (sign == 1) return 0;  // 既に反時計回りがあったら凹
            sign = -1;
        }
    }
    return 1;
}

// X座標優先、次いでY座標で比較するソート用関数
bool compare_xy(const Point& a, const Point& b) {
    if (std::abs(a.x - b.x) > EPS) return a.x < b.x;
    return a.y < b.y;
}
// (Y座標優先)
bool compare_y(const Point& a, const Point& b) {
    return a.y < b.y;
}
// ==========================================
// 凸包 (Convex Hull)
// アンドリューのアルゴリズム (計算量: O(N log N))
// ==========================================
vector<Point> convexHull(vector<Point> p) {
    int n = p.size();
    if (n < 3) return p; // 3点未満ならそのまま返す
    
    // 1. 点を辞書順ソート
    sort(p.begin(), p.end(), compare_xy);
    
    vector<Point> ch(2 * n);
    int k = 0; // 凸包の頂点数
    
    // 2. 下側凸包の構築
    // 外積が負(時計回り)の間、直前の頂点を削除し続ける
    // ※ 同一直線上の点を含めたい場合は `< -EPS` を `< EPS` に変更する
    for (int i = 0; i < n; ++i) {
        while (k > 1 && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < -EPS) {
            k--;
        }
        ch[k++] = p[i];
    }
    
    // 3. 上側凸包の構築
    for (int i = n - 2, t = k; i >= 0; --i) {
        while (k > t && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < -EPS) {
            k--;
        }
        ch[k++] = p[i];
    }
    
    // 最後に追加される始点は重複するためサイズを -1 する
    ch.resize(k - 1);
    
    return ch; // 構築された凸包の頂点列 (反時計回り)
}
// ==========================================
// 凸多角形の直径 (最遠点対の距離)
// キャリパー法 (計算量: O(N))
// ※ poly は頂点が反時計回りに並んだ凸多角形であること
// ==========================================
double convexDiameter(const std::vector<Point>& poly) {
    int n = poly.size();
    if (n == 0) return 0.0;
    if (n == 1) return 0.0;
    if (n == 2) return abs(poly[0] - poly[1]);

    // 1. x座標が最小の点(i)と最大の点(j)を見つける
    int i = 0, j = 0;
    for (int k = 1; k < n; ++k) {
        if (poly[k].x < poly[i].x) i = k;
        if (poly[k].x > poly[j].x) j = k;
    }

    double max_dist = 0.0;
    int si = i, sj = j; // 開始位置を記憶

    // 2. キャリパーを回転させながら最遠点対を探す
    // 180度回転するまで（開始位置に逆向きで戻るまで）ループ
    while (i != sj || j != si) {
        max_dist = std::max(max_dist, abs(poly[i] - poly[j]));

        // 次の辺へのベクトルの外積を利用して、どちらの点を進めるか決定する
        // poly[i]->poly[i+1] のベクトルと、poly[j]->poly[j+1] のベクトルを比較
        if (cross(poly[(i + 1) % n] - poly[i], poly[(j + 1) % n] - poly[j]) < 0) {
            // i 側の辺の方が角度が浅い（先にキャリパーが辺に重なる）ため、i を進める
            i = (i + 1) % n;
        } else {
            // そうでなければ j を進める
            j = (j + 1) % n;
        }
    }
    
    return max_dist;
}
// ==========================================
// 凸多角形の切断 (Convex Cut)
// 直線 p1-p2 で切断し、その進行方向「左側」に残る凸多角形を返す
// ==========================================
std::vector<Point> convexCut(const std::vector<Point>& poly, Point p1, Point p2) {
    std::vector<Point> res;
    int n = poly.size();
    
    // 直線の方向ベクトル
    Point v = p2 - p1;

    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];

        // p1 から a, b へのベクトルと、直線ベクトル v の外積
        // 正なら左側、負なら右側、0なら直線上
        double c1 = cross(v, a - p1);
        double c2 = cross(v, b - p1);

        // 1. 点 a が直線の左側、または直線上にある場合は追加
        if (c1 > -EPS) {
            res.push_back(a);
        }

        // 2. 辺 a-b が直線と厳密に交差する場合 (c1 と c2 が異符号) は交点を追加
        if (c1 * c2 < -EPS) {
            double d1 = std::abs(c1);
            double d2 = std::abs(c2);
            // 面積比で内分点（交点）を求める
            double t = d1 / (d1 + d2);
            res.push_back(a + (b - a) * t);
        }
    }
    
    return res;
}

// ==========================================
// 最近点対の距離を求める再帰関数
// ==========================================
double closestPairRec(std::vector<Point>& pts, int l, int r) {
    // 領域内の点が1個以下の場合は無限大を返す
    if (r - l <= 1) return INF;
    
    // 中央で分割
    int m = l + (r - l) / 2;
    double mid_x = pts[m].x;
    
    // 左右の領域を再帰的に計算し、暫定の最小距離 d を取得
    double d = std::min(closestPairRec(pts, l, m), closestPairRec(pts, m, r));
    
    // 分割ライン周辺（距離 d 未満）の点を strip 配列に集める
    std::vector<Point> strip;
    for (int i = l; i < r; ++i) {
        if (std::abs(pts[i].x - mid_x) < d) {
            strip.push_back(pts[i]);
        }
    }
    
    // 集めた点を Y 座標でソート
    std::sort(strip.begin(), strip.end(), compare_y);
    
    // strip 内の点ペアについて距離を計算
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size(); ++j) {
            // Y 座標の差が暫定最小距離 d 以上になったら、
            // それ以上調べる必要はない（枝刈り）
            if (strip[j].y - strip[i].y >= d) break;
            
            d = std::min(d, abs(strip[i] - strip[j]));
        }
    }
    
    return d;
}

// ==========================================
// ラッパー関数
// ==========================================
double getClosestPairDistance(std::vector<Point>& pts) {
    // 最初に X 座標優先でソートしておく
    std::sort(pts.begin(), pts.end(), compare_xy);
    return closestPairRec(pts, 0, pts.size());
}

// 点 p が円 c の内部 (境界含む) にあるか判定
bool in_circle(const Circle& c, const Point& p) {
    return abs(c.c - p) <= c.r + EPS;
}

// 2点を直径の端点とする円を構築
Circle circle_2(const Point& a, const Point& b) {
    Point c = (a + b) / 2.0;
    return {c, abs(a - b) / 2.0};
}

// 3点を通る円 (外接円) を構築
Circle circle_3(const Point& a, const Point& b, const Point& c) {
    Point ab = b - a;
    Point ac = c - a;
    double d = 2.0 * (ab.x * ac.y - ab.y * ac.x);
    
    // 3点が同一直線上にある場合のフェイルセーフ (通常はWelzl法内で弾かれます)
    if (abs(d) < EPS) {
        double d1 = abs(a - b), d2 = abs(b - c), d3 = abs(c - a);
        if (d1 >= d2 && d1 >= d3) return circle_2(a, b);
        if (d2 >= d1 && d2 >= d3) return circle_2(b, c);
        return circle_2(c, a);
    }
    
    // 外心 (Circumcenter) の計算
    double ab_norm = norm(ab);
    double ac_norm = norm(ac);
    Point p;
    p.x = (ac.y * ab_norm - ab.y * ac_norm) / d;
    p.y = (ab.x * ac_norm - ac.x * ab_norm) / d;
    p = p + a; // 原点を a に移動して計算したので戻す
    
    return {p, abs(p - a)};
}

// ==========================================
// Welzlのアルゴリズム (再帰部分)
// P: 全頂点の配列, R: 円周上にあることが確定した点の配列, n: 現在見ているPの要素数
// ==========================================
Circle welzl_helper(vector<Point>& P, vector<Point> R, int n) {
    // ベースケース: 見るべき点がない、または境界の点が3つ確定した
    if (n == 0 || R.size() == 3) {
        if (R.size() == 0) return {{0, 0}, 0};
        if (R.size() == 1) return {R[0], 0};
        if (R.size() == 2) return circle_2(R[0], R[1]);
        return circle_3(R[0], R[1], R[2]);
    }

    Point p = P[n - 1];
    
    // 1つ前の点までで作った最小包含円を取得
    Circle c = welzl_helper(P, R, n - 1);

    // 新しい点 p が円の内部にあれば、円を更新する必要なし
    if (in_circle(c, p)) return c;

    // p が円の外部にあるなら、p は必ず新しい最小包含円の境界上にある
    R.push_back(p);
    return welzl_helper(P, R, n - 1);
}

// ==========================================
// すべての点を内包する最小の円を求める関数
// ==========================================
Circle getMinEnclosingCircle(vector<Point> P) {
    if (P.empty()) return {{0, 0}, 0};
    
    // ランダムシャッフル (期待値 O(N) にするために必須)
    random_device rd;
    mt19937 gen(rd());
    shuffle(P.begin(), P.end(), gen);
    
    return welzl_helper(P, vector<Point>(), P.size());
}

// ==========================================
// Binary Indexed Tree (BIT) / Fenwick Tree
// 1-indexed で実装
// ==========================================
struct BIT {
    int n;
    vector<int> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}
    
    void add(int i, int x) {
        for (; i <= n; i += i & -i) tree[i] += x;
    }
    
    int sum(int i) {
        int s = 0;
        for (; i > 0; i -= i & -i) s += tree[i];
        return s;
    }
    
    // 区間 [l, r] の和
    int query(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

// ==========================================
// イベント構造体 (double 対応版)
// ==========================================
struct Event {
    double x;
    int type;      // 1: 左端, 2: 垂直線, 3: 右端
    double y1, y2;
    
    bool operator<(const Event& e) const {
        // X座標の比較 (EPSを考慮)
        if (std::abs(x - e.x) > EPS) return x < e.x;
        // X座標が同じ場合は type 順 (1 -> 2 -> 3)
        return type < e.type;
    }
};

// ==========================================
// 直交線分の交点列挙 (Point 構造体版)
// ==========================================
int countIntersections(const vector<pair<Point, Point>>& segments) {
    vector<Event> events;
    vector<double> y_coords; // 座標圧縮用のY座標リスト (double)

    // イベントの構築
    for (const auto& seg : segments) {
        double x1 = seg.first.x, y1 = seg.first.y;
        double x2 = seg.second.x, y2 = seg.second.y;
        
        // Y座標が等しい（誤差範囲内）なら水平線
        if (std::abs(y1 - y2) < EPS) {
            if (x1 > x2) swap(x1, x2);
            events.push_back({x1, 1, y1, y1}); // 左端
            events.push_back({x2, 3, y1, y1}); // 右端
            y_coords.push_back(y1);
        } 
        // そうでなければ垂直線
        else {
            if (y1 > y2) swap(y1, y2);
            events.push_back({x1, 2, y1, y2}); // 垂直線クエリ
            y_coords.push_back(y1);
            y_coords.push_back(y2);
        }
    }

    // Y座標の座標圧縮
    sort(y_coords.begin(), y_coords.end());
    
    // double 型の unique には EPS を考慮したラムダ式が必要
    y_coords.erase(unique(y_coords.begin(), y_coords.end(), [](double a, double b) {
        return std::abs(a - b) < EPS;
    }), y_coords.end());
    
    // 圧縮された座標(1-indexed)を取得する関数
    // lower_bound も誤差を考慮して少し小さい値で探索するテクニックを使用
    auto get_compressed_y = [&](double y) {
        auto it = lower_bound(y_coords.begin(), y_coords.end(), y - EPS);
        return (it - y_coords.begin()) + 1;
    };

    // イベントをX座標順にソート
    sort(events.begin(), events.end());

    BIT bit(y_coords.size());
    long long total_intersections = 0;

    // 平面走査
    for (const auto& ev : events) {
        if (ev.type == 1) {
            bit.add(get_compressed_y(ev.y1), 1);
        } else if (ev.type == 2) {
            int cy1 = get_compressed_y(ev.y1);
            int cy2 = get_compressed_y(ev.y2);
            total_intersections += bit.query(cy1, cy2);
        } else if (ev.type == 3) {
            bit.add(get_compressed_y(ev.y1), -1);
        }
    }

    return total_intersections;
}
// ==========================================
// 三角形の内接円 (Incircle)
// ==========================================
Circle getIncircle(Point a, Point b, Point c) {
    // 1. 各辺の長さを計算
    double len_a = abs(b - c);
    double len_b = abs(c - a);
    double len_c = abs(a - b);
    
    // 周囲の長さ
    double perimeter = len_a + len_b + len_c;
    
    // 3点が同一直線上にある（三角形をなさない）場合のフェイルセーフ
    if (perimeter < 1e-10) return {{0, 0}, 0};
    
    // 2. 内心 (加重平均) の計算
    Point center;
    center.x = (len_a * a.x + len_b * b.x + len_c * c.x) / perimeter;
    center.y = (len_a * a.y + len_b * b.y + len_c * c.y) / perimeter;
    
    // 3. 半径の計算 (面積 / 周囲の長さ)
    // 面積 S の2倍 = |cross(b - a, c - a)|
    // r = (2 * S) / perimeter
    double r = std::abs(cross(b - a, c - a)) / perimeter;
    
    return {center, r};
}

// ==========================================
// 三角形の外接円 (Circumcircle)
// ==========================================
Circle getCircumcircle(Point a, Point b, Point c) {
    // 頂点 A を原点 (0,0) とするベクトルに変換
    Point ab = b - a;
    Point ac = c - a;
    
    // 2つのベクトルの外積の2倍を計算 (分母になります)
    double d = 2.0 * (ab.x * ac.y - ab.y * ac.x);
    
    // 3点が同一直線上にある場合 (外積がほぼ0、三角形をなさない) のフェイルセーフ
    if (std::abs(d) < 1e-10) return {{0, 0}, 0};
    
    // ベクトルの長さの2乗
    double norm_ab = norm(ab);
    double norm_ac = norm(ac);
    
    // 外心 (Circumcenter) の座標を計算
    Point center;
    center.x = (ac.y * norm_ab - ab.y * norm_ac) / d;
    center.y = (ab.x * norm_ac - ac.x * norm_ab) / d;
    
    // 原点を A としていたので、実際の座標系に戻す
    center = center + a;
    
    // 半径は外心から任意の頂点 (例えば a) までの距離
    double r = abs(center - a);
    
    return {center, r};
}

int main(){
    Point a,b,c;
    cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
    Circle ans = getIncircle(a,b,c);
    printf("%.10f %.10f %.10f\n",ans.c.x,ans.c.y,ans.r);
}