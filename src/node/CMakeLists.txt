file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_library( node
        ${SOURCES}
        ${HEADERS} )

target_link_libraries(node paxosmecore)
target_include_directories(node PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")