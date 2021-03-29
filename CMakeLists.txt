cmake_minimum_required(VERSION 3.16)

project(paxosme)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)