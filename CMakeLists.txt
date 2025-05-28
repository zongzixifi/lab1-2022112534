cmake_minimum_required(VERSION 3.30)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

# Graphviz
set(GRAPHVIZ_INCLUDE_DIR "/opt/homebrew/include/graphviz")
set(GRAPHVIZ_LIB_DIR "/opt/homebrew/lib")
include_directories(${GRAPHVIZ_INCLUDE_DIR})
link_directories(${GRAPHVIZ_LIB_DIR})

add_library(corelib
        lab1.cpp
        lab1.h)

# 主程序
add_executable(lab1
        main.cpp
)

target_link_libraries(lab1 corelib cgraph gvc)

# 单元测试
enable_testing()
include(GoogleTest)

add_subdirectory(googletest)

add_executable(runTests
        tests/test_code.cpp
)

target_include_directories(runTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
)

target_link_libraries(runTests corelib gtest gtest_main cgraph gvc)

gtest_discover_tests(runTests)

# 代码覆盖率（可选）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping -O0 -g")