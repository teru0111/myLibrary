/*
[偏角ソート]
単位円をイメージする。
原点からの角度でソートする。
*/
struct point{ 
    ll x,y;
    point operator+(const point& p) const { return {x + p.x, y + p.y}; }
    point operator-(const point& p) const { return {x - p.x, y - p.y}; }
};
ll cross(const point &a, const point &b){
    return (ll)a.x*b.y-(ll)a.y*b.x;
}
ll dot(const point &a, const point &b){
    return (ll)a.x * b.x + (ll)a.y * b.y;
}
bool cmp(const point &a, const point &b){
    ll ah = (a.y < 0 or (a.y == 0 and a.x < 0));
    ll bh = (b.y < 0 or (b.y == 0 and b.x < 0));
    if (ah != bh) return ah < bh; // 上下どちらの半円に位置するか
    return cross(a, b) > 0; // 外積
}
void argument_sort(vector<point> &points){
    sort(points.begin(),points.end(),cmp);
}
// 2つのベクトルのなす角(0度〜180度)を返す関数
double get_angle(const point &a, const point &b) {
    double angle = atan2(a.y, a.x) - atan2(b.y, b.x);
    while (angle < 0) angle += 2 * PI;
    while (angle > 2 * PI) angle -= 2 * PI;
    if (angle > PI) angle = 2 * PI - angle;
    return angle * 180.0 / PI; // 度数法に変換
}
