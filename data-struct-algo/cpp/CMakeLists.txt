cmake_minimum_required(VERSION 3.27)
project(data-struct-algo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE srcs "src/*.cpp")

foreach(file_path ${srcs})
    get_filename_component(file_stem ${file_path} NAME_WLE)
    add_executable(${file_stem} ${file_path})
endforeach()
