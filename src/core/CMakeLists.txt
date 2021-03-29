file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
message(STATUS "_PROTOBUF_PROTOC: ${_PROTOBUF_PROTOC}")
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/proto/state.proto)

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(paxosmecore
        ${HEADERS}
        ${SOURCES}
        ${PROTO_HEADER}
        ${PROTO_SRC})

target_include_directories(paxosmecore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

