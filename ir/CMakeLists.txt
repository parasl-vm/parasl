# set(IR_SOURCES )

# add_library(ir SHARED ${IR_SOURCES})

# target_include_directories(ir ${PROJECT_SOURCE_DIR})


cmake_minimum_required(VERSION 3.16)
project(ir)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g -O0)

add_executable(ir main.cpp vreg.cpp function.cpp inst.cpp)