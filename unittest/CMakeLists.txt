cmake_minimum_required(VERSION 3.16)

project(paxosme_ut)

set(CMAKE_CXX_STANDARD 14)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_subdirectory(${ROOT_DIR}/3rd_party/gtest gtest)
include_directories(${ROOT_DIR}/include)

add_subdirectory(${ROOT_DIR}/3rd_party/glog glog)

add_subdirectory(storage)