cmake_minimum_required(VERSION 3.16)

project(paxosme)

set(CMAKE_CXX_STANDARD 14)

# common include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# leveldb include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/leveldb/include)
add_subdirectory(3rd_party/leveldb)

add_subdirectory(src)
add_subdirectory(ut)