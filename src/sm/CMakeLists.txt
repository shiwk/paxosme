
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/proto/kv.proto)

add_library(sm
        ${HEADERS}
        ${SOURCES}
        ${PROTO_HEADER}
        ${PROTO_SRC})



target_include_directories(sm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_include_directories(sm PUBLIC ${PROTOBUF_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(sm PRIVATE leveldb)## leveldb PRIVATE?