cmake_minimum_required(VERSION 3.25)

project(Ether)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE Debug)

include_directories(.)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(ether ether.cpp
                LibEther/AST.cpp
                LibEther/DataTypes.cpp
                LibEther/Parser.cpp
                LibEther/Tokenizer.cpp
)

