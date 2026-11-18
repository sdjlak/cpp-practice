#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 3;
    vector<int> a(n), b(n);

    // 错误示范：x 是拷贝，cin >> x 只写进临时副本，a 不变
    for (auto x : a) cin >> x;
    cout << "copy  : ";
    for (auto x : a) cout << x << ' ';
    cout << '\n';

    // 正确示范：x 是引用，cin >> x 直接写进 b 的元素
    for (auto &x : b) cin >> x;
    cout << "ref   : ";
    for (auto x : b) cout << x << ' ';
    cout << '\n';
    return 0;
}
