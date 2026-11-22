# CMake 工程配置：一条命令构建所有目标
#
# 用法（在 D:\c++ 下执行）：
#   cmake -S string_lab -B string_lab/build        # 生成构建文件
#   cmake --build string_lab/build                 # 编译
#   ctest --test-dir string_lab/build --output-on-failure   # 跑测试
#
# 或者直接构建并测试：
#   cmake --build string_lab/build && ctest --test-dir string_lab/build --output-on-failure

cmake_minimum_required(VERSION 3.16)
project(string_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

enable_testing()

# ---- 核心库：String 类 ----
add_library(my_string STATIC src/string.cpp)
target_include_directories(my_string PUBLIC include)

# ---- 测试 1：构造 / 析构 / 拷贝构造 ----
add_executable(test_basic tests/test_basic.cpp)
target_link_libraries(test_basic PRIVATE my_string)
add_test(NAME basic COMMAND test_basic)

# ---- 测试 2：拷贝赋值运算符 ----
add_executable(test_assign tests/test_assign.cpp)
target_link_libraries(test_assign PRIVATE my_string)
add_test(NAME assign COMMAND test_assign)
