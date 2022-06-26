#list(APPEND CMAKE_PREFIX_PATH ../../3rd_party/grpc)
set(GRPC_AS_SUBMODULE TRUE)
set(GRPC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd_party/grpc)
include(cmake/common.cmake)


# Proto file
set(PROTO_NAME paxosme)
get_filename_component(proto "./protos/${PROTO_NAME}.proto" ABSOLUTE)
get_filename_component(proto_path "${proto}" PATH)

# Generated sources
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/protogen)

set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/protogen/${PROTO_NAME}.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/protogen/${PROTO_NAME}.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/protogen/${PROTO_NAME}.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/protogen/${PROTO_NAME}.grpc.pb.h")

#INCLUDE(FindProtobuf)
#FIND_PACKAGE(Protobuf REQUIRED)


add_custom_command(
        OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/protogen"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/protogen"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${proto}"
        DEPENDS "${proto}")

# Include generated *.pb.h files
#include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

#find_package(Protobuf REQUIRED)
message(STATUS ${_PROTOBUF_LIBPROTOBUF})
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../*.cpp)

# grpc_proto
add_library(network_grpc
        ${grpc_srcs}
        ${grpc_hdrs}
        ${proto_srcs}
        ${proto_hdrs}
        ${SOURCES}
        ${HEADERS})

target_link_libraries(network_grpc
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        paxosmecore)

target_include_directories(network_grpc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/ )
target_include_directories(network_grpc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
target_include_directories(network_grpc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_BINARY_DIR}/protogen)