cmake_minimum_required(VERSION 3.10)
project(VibeFinder CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
else()
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_executable(vibefinder
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/BTree.cpp
        ${SRC_DIR}/Benchmark.cpp
        ${SRC_DIR}/CsvLoader.cpp
        ${SRC_DIR}/RedBlackTree.cpp)

target_include_directories(vibefinder PRIVATE ${SRC_DIR})