cmake_minimum_required(VERSION 3.31)
project(scale LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Werror -Wall -Wextra -pedantic -Wunused)

add_executable(balanced_scale balanced_scale.cpp)
