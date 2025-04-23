cmake_minimum_required(VERSION 3.30)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

# 手动设置 Graphviz 路径
set(GRAPHVIZ_INCLUDE_DIR "/opt/homebrew/include/graphviz")
set(GRAPHVIZ_LIB_DIR "/opt/homebrew/lib")

include_directories(${GRAPHVIZ_INCLUDE_DIR})
link_directories(${GRAPHVIZ_LIB_DIR})

add_executable(lab1 main.cpp lab1.h)

# 链接 Graphviz 所需库（以 dot 为主）
target_link_libraries(lab1 cgraph gvc)

