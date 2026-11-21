// String 类练习的测试程序。不要修改这个文件，只改 string.cpp。
//
// 编译（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra -g string_lab/string.cpp string_lab/main.cpp -o string_lab/demo.exe
//
// 运行：
//   .\string_lab\demo.exe
//
// 全过会打印 "ALL PASSED"，退出码 0。

#include "string.h"

#include <cstdio>
#include <cstring>

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
    std::printf("==== Test 1: default ctor ====\n");
    {
        String a;
        CHECK(a.size() == 0);
        CHECK(a.c_str() != nullptr);
        CHECK(std::strlen(a.c_str()) == 0);
        CHECK(String::alive() == 1);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test 2: ctor from C string ====\n");
    {
        String b("hello");
        CHECK(b.size() == 5);
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        CHECK(String::alive() == 1);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test 3: copy ctor must be deep ====\n");
    {
        String a("hello");
        String b(a);
        CHECK(b.size() == a.size());
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        // 地址相同 = 浅拷贝，两个对象共用一块内存，析构时会 double free
        CHECK(b.c_str() != a.c_str());
        CHECK(String::alive() == 2);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test 4: empty string and nullptr ====\n");
    {
        String e("");
        CHECK(e.size() == 0);
        CHECK(std::strlen(e.c_str()) == 0);

        String n(nullptr);
        CHECK(n.size() == 0);
        CHECK(std::strlen(n.c_str()) == 0);

        CHECK(String::alive() == 2);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test 5: lifetime independence (the real test) ====\n");
    {
        String* survivor = nullptr;
        {
            String source("hello");
            survivor = new String(source);
        }  // source 在这里析构；如果拷贝构造是浅的，survivor 现在指向野内存
        CHECK(survivor != nullptr);
        CHECK(survivor->size() == 5);
        CHECK(std::strcmp(survivor->c_str(), "hello") == 0);
        delete survivor;
    }
    CHECK(String::alive() == 0);

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
