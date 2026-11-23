// 0002 · 小中大（CCF CSP 2019.03 第一题，简化版）
//
// 这道题的风格，就是 CSP 第 1 题的典型样子：题目短、逻辑简单，
// 但细节一堆，稍不注意就丢分。
//
// 题目：
//   给定 n 个已经按升序排好的整数 a[0..n-1]，
//   输出「最大值、中位数、最小值」，用空格分隔。
//
// 中位数的规则（这是本题唯一的坑）：
//   n 为奇数 → 正中间那个数，例如 n=5 取 a[2]
//   n 为偶数 → 中间两个数的平均值；平均值是整数就输出整数，是 .5 就输出一位小数
//
// 示例：
//   n=5, [1,2,3,4,5]  ->  "5 3 1"
//   n=4, [1,2,3,4]    ->  "4 2.5 1"
//   n=4, [1,3,5,7]    ->  "7 4 1"      ← 平均是整数 4，不能输出 4.0
//   n=1, [42]         ->  "42 42 42"
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0002_median.cpp -o algorithms/0002.exe
//   .\algorithms\0002.exe
//
// 提示：这个场景用 double 是安全的——0.5 在二进制里能被精确表示。
//   唯一要小心的是：结果是整数时要输出 "4"，不能输出 "4.0"。

#include <cstdio>
#include <string>
#include <vector>
using namespace std;
// 返回形如 "5 3 1" 的字符串（最大值 中位数 最小值）
std::string small_middle_big(const std::vector<int>& a) {
    int mn = a[0];
    int z = a.size();
    int i;
    string mid;
    if (z % 2 == 0) {
        int j = z / 2;
        i = a[j - 1] + a[j];
        if (i % 2 == 0) {
            mid = to_string(i / 2);
        }
        else {
            int q = i / 2;
            if (q == 0 && i < 0) {
                mid = "-0.5";
            }
            else {
                mid = to_string(q) + ".5";
            }
        }
    }
    else {
        mid = to_string(a[z / 2]);
    }
    return to_string(a[z - 1]) + " " + mid + " " + to_string(a[0]);
}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void expect(const std::vector<int>& a, const std::string& want, const char* label) {
    std::string got = small_middle_big(a);
    bool ok = (got == want);
    std::printf("  [%s] %s\n", ok ? "PASS" : "FAIL", label);
    if (!ok) {
        std::printf("       got  \"%s\"\n", got.c_str());
        std::printf("       want \"%s\"\n", want.c_str());
        ++failed;
    }
}

int main() {
    expect({1, 2, 3, 4, 5}, "5 3 1", "n=5 奇数，中位数无小数");
    expect({1, 2, 3, 4}, "4 2.5 1", "n=4 偶数，中位数 .5");
    expect({1, 3, 5, 7}, "7 4 1", "n=4 偶数，中位数是整数");
    expect({7, 7, 7}, "7 7 7", "n=3 全相同");
    expect({42}, "42 42 42", "n=1 单元素");
    expect({-5, -1, 0, 3}, "3 -0.5 -5", "含负数，中位数 -0.5");
    expect({0, 0, 1, 100}, "100 0.5 0", "含 0，中位数 0.5");

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
