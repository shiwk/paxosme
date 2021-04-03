file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library( node
        ${SOURCES}
        ${HEADERS} )

target_link_libraries(node paxosmecore)
target_link_libraries(node network_grpc)
target_include_directories(node PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")