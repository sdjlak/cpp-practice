// LeetCode 1. 两数之和
//
// 题目：给定一个整数数组 nums 和一个目标值 target，
//       请你在该数组中找出「和为目标值」的那两个整数，并返回它们的下标。
//       假设每种输入只会对应一个答案，同一个元素不能重复使用。
//
// 示例：
//   nums = [2, 7, 11, 15], target = 9   ->  [0, 1]   （2 + 7 = 9）
//   nums = [3, 2, 4],      target = 6   ->  [1, 2]
//   nums = [3, 3],         target = 6   ->  [0, 1]
//
// 编译（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0001_two_sum.cpp -o algorithms/0001.exe
//
// 运行：
//   .\algorithms\0001.exe

#include <cstdio>
#include <unordered_map>
#include <vector>
using namespace std;
// TODO: 用哈希表实现。思路提示：
//   边遍历边记「这个数 -> 它的下标」；
//   每走到一个数 x，先查 target - x 有没有在表里，有就直接返回；
//   没有再把 x 记进去。
//   这样只扫一遍，O(n)。
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    // 在这里写你的代码
    unordered_map<int,int>a;
    for (int i = 0;i < nums.size();i++) {
        int need = target - nums[i];
        auto it = a.find(need);
        if (it != a.end()) {
            return {a[need], i};
        }
        a[nums[i]] = i;


        }
     
    return {};
}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void check(bool cond, const char* what, int line) {
    if (cond) {
        std::printf("  [PASS] %s\n", what);
    } else {
        std::printf("  [FAIL] %s   (line %d)\n", what, line);
        ++failed;
    }
}

#define CHECK(cond) check((cond), #cond, __LINE__)

static void expect(const std::vector<int>& got, std::vector<int> want) {
    // 两个下标顺序无所谓，统一排序后比较
    std::vector<int> g = got;
    if (g.size() == 2 && g[0] > g[1]) {
        int t = g[0];
        g[0] = g[1];
        g[1] = t;
    }
    check(g == want, "twoSum result matches", __LINE__);
    if (!(g == want)) {
        std::printf("       got: [");
        for (size_t i = 0; i < g.size(); ++i) std::printf("%s%d", i ? ", " : "", g[i]);
        std::printf("]  want: [%d, %d]\n", want[0], want[1]);
    }
}

int main() {
    std::printf("==== 样例 1 ====\n");
    expect(twoSum({2, 7, 11, 15}, 9), {0, 1});

    std::printf("==== 样例 2 ====\n");
    expect(twoSum({3, 2, 4}, 6), {1, 2});

    std::printf("==== 样例 3：两个相同的数 ====\n");
    expect(twoSum({3, 3}, 6), {0, 1});

    std::printf("==== 样例 4：没有答案 ====\n");
    CHECK(twoSum({1, 2, 3}, 100).empty());

    std::printf("==== 样例 5：负数 ====\n");
    expect(twoSum({-3, 4, 3, 90}, 0), {0, 2});

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
