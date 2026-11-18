// 两端取牌最大得分 —— 优化版
// 优化点：
// 1) 不存前缀和，滚动维护「左边 i 个 + 右边 k-i 个」的和，额外空间 O(1)；
// 2) 两个等价视角：
//      A. 枚举左边 i 个（k+1 种）
//      B. 总和 - 长度 n-k 的连续子段的最小和（n-k+1 种）
//    按 k 与 n-k 的大小选循环更少的一侧，最多约 n/2+1 次迭代；
// 3) 边界防御：k=0、k>n 都不会越界。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> a(n);
    for (auto &x : a) cin >> x;

    if (k > n) k = n;               // 题意保证 k <= n，这里只做防御
    if (k == 0) { cout << 0 << '\n'; return 0; }

    if (k <= n - k) {
        // 视角 A：枚举「左边 i 个 + 右边 k-i 个」，k+1 种
        long long sum = 0;
        for (int i = n - k; i < n; ++i) sum += a[i];   // i = 0：右边取 k 个
        long long ans = sum;
        for (int i = 1; i <= k; ++i) {
            sum += a[i - 1] - a[n - k + i - 1];        // 左边进一个、右边出一个
            if (sum > ans) ans = sum;
        }
        cout << ans << '\n';
    } else {
        // 视角 B：答案 = 总和 - 长度 n-k 的连续子段最小和，n-k+1 种
        int m = n - k;
        long long total = 0;
        for (long long x : a) total += x;

        long long w = 0;
        for (int i = 0; i < m; ++i) w += a[i];
        long long minW = w;
        for (int i = m; i < n; ++i) {
            w += a[i] - a[i - m];
            if (w < minW) minW = w;
        }
        cout << (total - minW) << '\n';
    }
    return 0;
}
