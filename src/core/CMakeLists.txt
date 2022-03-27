file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)

#set(PROTOBUF_AS_SUBMODULE TRUE)
#set(PROTOBUF_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../3rd_party/grpc/third_party/protobuf)
#include(cmake/common.cmake)
#
#set(PROTO_NAME state)
#get_filename_component(proto "./proto/${PROTO_NAME}.proto" ABSOLUTE)
#get_filename_component(proto_path "${proto}" PATH)
#
#set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc")
#set(PROTO_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h")
#message(STATUS ${_PROTOBUF_LIBPROTOBUF})
#
#add_custom_command(
#        OUTPUT "${PROTO_SRC}" "${PROTO_HEADER}"
#        COMMAND ${_PROTOBUF_PROTOC}
#        ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#        -I "${proto_path}"
#        "${proto}"
#        DEPENDS "${proto}")

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
message(STATUS "_PROTOBUF_PROTOC: ${_PROTOBUF_PROTOC}")
#INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/proto/state.proto)


# Include generated *.pb.h files
#include_directories("${CMAKE_CURRENT_BINARY_DIR}")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(paxosmecore
        ${HEADERS}
        ${SOURCES}
        ${PROTO_HEADER}
        ${PROTO_SRC})

target_link_libraries(paxosmecore util)
target_link_libraries(paxosmecore sm)
target_link_libraries(paxosmecore db)

target_include_directories(paxosmecore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}" ${PROTOBUF_INCLUDE_DIR})
target_include_directories(paxosmecore PRIVATE ${PROTOBUF_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)