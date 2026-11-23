// 移动语义测试。不要修改这个文件，只改 src/string.cpp。
//
// 编译（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra -g string_lab/src/string.cpp string_lab/tests/test_move.cpp -o string_lab/tests/test_move.exe
//
// 或者走 CMake：
//   cmake --build string_lab/build
//   ctest --test-dir string_lab/build --output-on-failure

#include "my_string.h"

#include <cstdio>
#include <cstring>
#include <utility>

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

int main() {
    std::printf("==== Test A: move ctor ====\n");
    {
        String a("hello");
        String b(std::move(a));  // 把 a 的内存「偷」给 b
        CHECK(b.size() == 5);
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        CHECK(a.size() == 0);                  // a 被掏空
        CHECK(std::strlen(a.c_str()) == 0);    // a 仍是合法对象，c_str() 不能崩
        CHECK(a.c_str() != b.c_str());
        CHECK(String::alive() == 2);           // 确实新建了对象
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test B: move assignment ====\n");
    {
        String a("hello");
        String b("world");
        b = std::move(a);  // b 丢掉旧的 "world"，接管 a 的 "hello"
        CHECK(b.size() == 5);
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        CHECK(a.size() == 0);
        CHECK(std::strlen(a.c_str()) == 0);
        CHECK(String::alive() == 2);           // 赋值不改变对象数
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test C: self move assignment ====\n");
    {
        String a("hello");
        String* p = &a;
        a = std::move(*p);  // 自己偷自己，不能炸
        CHECK(a.size() == 5);
        CHECK(std::strcmp(a.c_str(), "hello") == 0);
        CHECK(String::alive() == 1);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test D: move from temporary (the real win) ====\n");
    {
        String a;
        a = String("temp");  // 临时对象，直接接管，不需要复制
        CHECK(a.size() == 4);
        CHECK(std::strcmp(a.c_str(), "temp") == 0);
        CHECK(String::alive() == 1);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test E: moved-from object is still usable ====\n");
    {
        String a("hello");
        String b(std::move(a));
        // 被掏空的对象仍然合法：可以重新赋值、可以正常析构
        a = String("again");
        CHECK(a.size() == 5);
        CHECK(std::strcmp(a.c_str(), "again") == 0);
        CHECK(b.size() == 5);
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        CHECK(String::alive() == 2);
    }
    CHECK(String::alive() == 0);

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
