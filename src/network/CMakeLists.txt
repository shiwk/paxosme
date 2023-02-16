file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp )
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

# grpc_proto
add_library(network
        ${SOURCES}
        ${HEADERS})

add_subdirectory(grpc)
add_subdirectory(asio)
target_include_directories(network PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
target_link_libraries(network PRIVATE network_grpc paxosmecore)