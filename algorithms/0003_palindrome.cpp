// 0003 · 回文数（LeetCode 9，CSP 热身难度）
//
// 题目：判断一个整数是不是回文数——正着读和倒着读一样。
//   121   是回文
//   1221  是回文
//   123   不是
//   -121  不是（负号让任何负数都不是回文）
//   0     是
//
// 要求：不要把它转成字符串（那太简单了），用「反转后一半」的整数做法。
//
// 思路提示：
//   每次循环：取 x 的最后一位接到 rev 上，同时把 x 去掉最后一位；
//   当 rev >= x 时停下。此时：
//     - 偶数位：rev 应该等于 x（如 1221 → x=12, rev=12）
//     - 奇数位：rev/10 应该等于 x（如 121 → x=1, rev=12 → 12/10 == 1）
//   注意 10、100 这类「末尾是 0」的数：会提前停，而且不是回文，想想怎么处理。
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0003_palindrome.cpp -o algorithms/0003.exe
//   .\algorithms\0003.exe

#include <cstdio>
#include<vector>
bool isPalindrome(int x) {
    
    if (x < 0) return false;                        

    if (x != 0 && x % 10 == 0) return false;        


    int rev = 0;

    while (x > rev) {                 

        rev = rev * 10 + x % 10;      

        x /= 10;                      

    }

    return x == rev || x == rev / 10;
    

}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void expect(int x, bool want, const char* label) {
    bool got = isPalindrome(x);
    std::printf("  [%s] %s\n", got == want ? "PASS" : "FAIL", label);
    if (got != want) {
        std::printf("       x=%d  got %s, want %s\n", x,
                    got ? "true" : "false", want ? "true" : "false");
        ++failed;
    }
}

int main() {
    expect(121, true, "121 是回文");
    expect(1221, true, "1221 是回文");
    expect(123, false, "123 不是回文");
    expect(-121, false, "负数不是回文");
    expect(0, true, "0 是回文");
    expect(10, false, "10 不是回文（末尾为 0 的坑）");
    expect(1001, true, "1001 是回文");
    expect(123454321, true, "123454321 是回文");
    expect(1234567899, false, "大数，非回文");

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
