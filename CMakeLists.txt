cmake_minimum_required(VERSION 3.25)

project(Ether)
# set(CMAKE_BUILD_TYPE Debug)

include_directories(.)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(ether ether.cpp
                LibEther/AST.cpp
)

