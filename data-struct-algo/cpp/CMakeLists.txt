cmake_minimum_required(VERSION 3.27)
project(data-struct-algo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(brackets brackets.cpp)
add_executable(phone_lookup phone_lookup.cpp)
add_executable(poly poly.cpp)
