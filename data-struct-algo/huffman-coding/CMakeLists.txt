cmake_minimum_required(VERSION 3.27)
project(huffman-coding VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mycompress src/mycompress.cpp)
add_executable(myuncompress src/myuncompress.cpp)
add_executable(lucas src/lucas.cpp)
