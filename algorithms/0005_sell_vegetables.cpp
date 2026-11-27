// 0005 · 卖菜（CCF CSP 2018.09 第一题，真题）
//
// 题目：一条街上有 n 家店卖菜，第 i 家店第一天的菜价是 a[i]。
//       第二天，每家店的菜价 =「自己 + 相邻店」第一天价格的平均值，向下取整。
//       最左边和最右边的店只有一家邻居，中间的店有两家邻居。
//
// 输入样例：
//   8
//   4 1 3 1 6 5 17 9
// 输出样例：
//   2 2 1 3 4 9 10 13
//
// 解释（举例）：
//   第 2 家 = (4 + 1 + 3) / 3 = 2（向下取整）
//   第 1 家 = (4 + 1) / 2 = 2
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0005_sell_vegetables.cpp -o algorithms/0005.exe
//   .\algorithms\0005.exe
//
// 提示：
//   1) 第 1 家：只有右邻居；第 n 家：只有左邻居；中间：两个邻居；
//   2) 除法直接整数除就行（正数除法自动向下取整）；
//   3) 注意 n == 1 的边界：只有一家店时，第二天价格就是自己。

#include <cstdio>
#include <vector>

// 返回第二天的菜价
std::vector<int> next_day_price(const std::vector<int>& a) {
    int n = a.size();
    std::vector<int> b(n);
    if (n > 1) {
        b[0] = (a[0] + a[1]) / 2;
        b[n - 1] = (a[n - 2] + a[n - 1]) / 2;
        if (n > 2) {
            for (int i = 1;i < n - 1;i++) {
                b[i] = (a[i] + a[i - 1] + a[i + 1])/3;
            }
        }
    }
    else {
        return a;
    }
    
    return b;
}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void expect(const std::vector<int>& a, const std::vector<int>& want, const char* label) {
    std::vector<int> got = next_day_price(a);
    bool ok = (got == want);
    std::printf("  [%s] %s\n", ok ? "PASS" : "FAIL", label);
    if (!ok) {
        std::printf("       got : [");
        for (std::size_t i = 0; i < got.size(); ++i) std::printf("%s%d", i ? " " : "", got[i]);
        std::printf("]\n       want: [");
        for (std::size_t i = 0; i < want.size(); ++i) std::printf("%s%d", i ? " " : "", want[i]);
        std::printf("]\n");
        ++failed;
    }
}

int main() {
    expect({4, 1, 3, 1, 6, 5, 17, 9}, {2, 2, 1, 3, 4, 9, 10, 13}, "官方样例 n=8");
    expect({1, 3}, {2, 2}, "n=2 两端各只有一个邻居");
    expect({1, 2, 3}, {1, 2, 2}, "n=3 首尾特判");
    expect({5}, {5}, "n=1 只有一家店");
    expect({10, 10, 10, 10}, {10, 10, 10, 10}, "全部相同");

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
