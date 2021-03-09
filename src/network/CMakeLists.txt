#include(FetchContent)
#FetchContent_Declare(
#        gRPC
#        GIT_REPOSITORY https://github.com/grpc/grpc
##        GIT_TAG        v1.28.0
#)
#set(FETCHCONTENT_QUIET OFF)
#FetchContent_MakeAvailable(gRPC)
list(APPEND CMAKE_PREFIX_PATH ../../3rd_party/grpc)
set(GRPC_AS_SUBMODULE TRUE)
include(./common.cmake)

# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake "superbuild" file for C++ helloworld example.
# This build file demonstrates how to build the helloworld project
# and all its dependencies in a single cmake build (hence "superbuild")
# that is easy to build and maintain.
# cmake's ExternalProject_Add() is used to import all the sub-projects,
# including the "helloworld" project itself.
# See https://blog.kitware.com/cmake-superbuilds-git-submodules/

#cmake_minimum_required(VERSION 2.8)
#
## Project
#project(HelloWorld-SuperBuild C CXX)
#
#include(ExternalProject)
#
## Builds c-ares project from the git submodule.
## Note: For all external projects, instead of using checked-out code, one could
## specify GIT_REPOSITORY and GIT_TAG to have cmake download the dependency directly,
## without needing to add a submodule to your project.
#ExternalProject_Add(c-ares
#        PREFIX c-ares
#        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd_party/grpc/third_party/cares/cares"
#        CMAKE_CACHE_ARGS
#        -DCARES_SHARED:BOOL=OFF
#        -DCARES_STATIC:BOOL=ON
#        -DCARES_STATIC_PIC:BOOL=ON
#        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares
#        )
#
## Builds protobuf project from the git submodule.
#ExternalProject_Add(protobuf
#        PREFIX protobuf
#        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd_party/grpc/third_party/protobuf/cmake"
#        CMAKE_CACHE_ARGS
#        -Dprotobuf_BUILD_TESTS:BOOL=OFF
#        -Dprotobuf_WITH_ZLIB:BOOL=OFF
#        -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
#        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf
#        )
#
## Builds zlib project from the git submodule.
#ExternalProject_Add(zlib
#        PREFIX zlib
#        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd_party/grpc/third_party/zlib"
#        CMAKE_CACHE_ARGS
#        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib
#        )
#
## the location where protobuf-config.cmake will be installed varies by platform
#if (WIN32)
#    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake")
#else ()
#    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf")
#endif ()
#
## if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
#set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
#if (OPENSSL_ROOT_DIR)
#    set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
#endif ()
#
## Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
## are correctly located.
#ExternalProject_Add(grpc
#        PREFIX grpc
#        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd_party/grpc"
#        CMAKE_CACHE_ARGS
#        -DgRPC_INSTALL:BOOL=ON
#        -DgRPC_BUILD_TESTS:BOOL=OFF
#        -DgRPC_PROTOBUF_PROVIDER:STRING=package
#        -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
#        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
#        -DgRPC_ZLIB_PROVIDER:STRING=package
#        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
#        -DgRPC_CARES_PROVIDER:STRING=package
#        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
#        -DgRPC_SSL_PROVIDER:STRING=package
#        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
#        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc
#        DEPENDS c-ares protobuf zlib
#        )
message(STATUS "CMAKE_CURRENT_BINARY_DIR : ${CMAKE_CURRENT_BINARY_DIR}")
# Proto file
get_filename_component(hw_proto "./protos/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")


add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#find_package(Protobuf REQUIRED)
message(STATUS ${_PROTOBUF_LIBPROTOBUF})

# hw_grpc_proto
add_library(network
        ${hw_grpc_srcs}
        ${hw_grpc_hdrs}
        ${hw_proto_srcs}
        ${hw_proto_hdrs})

target_link_libraries(network
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

# Targets greeter_[async_](client|server)
#foreach(_target
#        greeter_client greeter_server
#        greeter_async_client greeter_async_client2 greeter_async_server)
#    add_executable(${_target} "${_target}.cc")
#    target_link_libraries(${_target}
#            hw_grpc_proto
#            ${_REFLECTION}
#            ${_GRPC_GRPCPP}
#            ${_PROTOBUF_LIBPROTOBUF})
#endforeach()