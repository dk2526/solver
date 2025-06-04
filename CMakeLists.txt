cmake_minimum_required(VERSION 3.31)
project(scale LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(balanced_scale balanced_scale.cpp)
