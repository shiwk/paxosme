
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# INCLUDE(FindProtobuf)
find_package(Protobuf REQUIRED)
# find_package(protobuf REQUIRED CONFIG)

# find_package(utf8_range REQUIRED)
# find_package(absl REQUIRED)
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/proto/kv.proto)

add_library(sm
        ${HEADERS}
        ${SOURCES}
        ${PROTO_HEADER}
        ${PROTO_SRC})

# set(protobuf_ABSL_USED_TARGETS
#     absl::absl_check
#     absl::absl_log
#     absl::algorithm
#     absl::base
#     absl::bind_front
#     absl::bits
#     absl::btree
#     absl::cleanup
#     absl::cord
#     absl::core_headers
#     absl::debugging
#     absl::die_if_null
#     absl::dynamic_annotations
#     absl::flags
#     absl::flat_hash_map
#     absl::flat_hash_set
#     absl::function_ref
#     absl::hash
#     absl::layout
#     absl::log_initialize
#     absl::log_severity
#     absl::memory
#     absl::node_hash_map
#     absl::node_hash_set
#     absl::optional
#     absl::span
#     absl::status
#     absl::statusor
#     absl::strings
#     absl::synchronization
#     absl::time
#     absl::type_traits
#     absl::utility
#     absl::variant
#   )


target_include_directories(sm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_include_directories(sm PUBLIC ${PROTOBUF_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(sm PRIVATE leveldb ${Protobuf_LIBRARIES})## leveldb PRIVATE?